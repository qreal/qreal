/* Copyright 2012-2017 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "twoDRobotModel.h"

#include <QtCore/QFile>

#include <qrkernel/platformInfo.h>
#include <qrkernel/settingsManager.h>
#include <kitBase/robotModel/robotParts/speaker.h>
#include <kitBase/robotModel/robotParts/motor.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <kitBase/robotModel/robotParts/colorSensorBlue.h>
#include <kitBase/robotModel/robotParts/colorSensorFull.h>
#include <kitBase/robotModel/robotParts/colorSensorGreen.h>
#include <kitBase/robotModel/robotParts/colorSensorPassive.h>
#include <kitBase/robotModel/robotParts/colorSensorRed.h>

#include "nxtDisplayWidget.h"
#include "robotModel/twoD/parts/twoDDisplay.h"
#include "robotModel/twoD/parts/twoDSpeaker.h"
#include "robotModel/twoD/parts/twoDMotor.h"

using namespace nxt::robotModel;
using namespace nxt::robotModel::twoD;
using namespace kitBase::robotModel;

TwoDRobotModel::TwoDRobotModel(RobotModelInterface &realModel)
	: twoDModel::robotModel::TwoDRobotModel(realModel)
	, mDisplayWidget(new NxtDisplayWidget())
{
}

robotParts::Device *TwoDRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Display>()) {
		return new parts::Display(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotParts::Speaker>()) {
		return new parts::TwoDSpeaker(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotParts::Motor>()) {
		return new parts::TwoDMotor(deviceInfo, port, *engine());
	}

	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

QString TwoDRobotModel::robotImage() const
{
	const QString key = "nxtRobot2DImage";
	const QString hackDefaultPath = "./images/nxt-robot.png";
	if (qReal::SettingsManager::value(key).isNull()) {
		qReal::SettingsManager::setValue(key, hackDefaultPath);
	}

	const QString settingsPath = qReal::PlatformInfo::invariantSettingsPath(key);
	return QFile::exists(settingsPath) ? settingsPath : ":/icons/nxt-robot.png";
}

PortInfo TwoDRobotModel::defaultLeftWheelPort() const
{
	return PortInfo("B", output);
}

PortInfo TwoDRobotModel::defaultRightWheelPort() const
{
	return PortInfo("C", output);
}

twoDModel::engine::TwoDModelDisplayWidget *TwoDRobotModel::displayWidget() const
{
	return mDisplayWidget;
}

QRect TwoDRobotModel::sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const
{
	if (deviceType.isA<robotParts::TouchSensor>()) {
		return QRect(-12, -5, 25, 10);
	}

	if (deviceType.isA<robotParts::ColorSensor>()
			|| deviceType.isA<robotParts::LightSensor>())
	{
		return QRect(-6, -6, 12, 12);
	}

	if (deviceType.isA<robotParts::RangeSensor>()) {
		return QRect(-20, -10, 40, 20);;
	}

	return QRect();
}

QPolygonF TwoDRobotModel::collidingPolygon() const
{
	QPolygonF res;
	res << QPointF(10, 10) << QPointF(42, 10) << QPointF(45, 20)
			<< QPointF(45, 30) << QPointF(42, 40) << QPointF(10, 40);
	return res;
}

qreal TwoDRobotModel::mass() const
{
	return 0.5;  /// @todo: measure it
}

qreal TwoDRobotModel::friction() const
{
	return 0.3;  /// @todo measure it
}

qreal TwoDRobotModel::onePercentAngularVelocity() const
{
	return 0.0055;
}

QList<QPointF> TwoDRobotModel::wheelsPosition() const
{
	return {QPointF(41, 5), QPointF(41, 45)};
}
