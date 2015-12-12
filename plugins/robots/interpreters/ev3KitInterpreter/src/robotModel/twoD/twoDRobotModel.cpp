/* Copyright 2007-2015 QReal Research Group
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

#include "src/ev3DisplayWidget.h"
#include "src/robotModel/twoD/parts/twoDDisplay.h"
#include "src/robotModel/twoD/parts/twoDSpeaker.h"
#include "src/robotModel/twoD/parts/twoDMotor.h"
#include "src/robotModel/twoD/parts/twoDLed.h"

using namespace ev3::robotModel;
using namespace ev3::robotModel::twoD;
using namespace kitBase::robotModel;

TwoDRobotModel::TwoDRobotModel(RobotModelInterface const &realModel)
	: twoDModel::robotModel::TwoDRobotModel(realModel)
	, mDisplayWidget(new Ev3DisplayWidget())
{
}

robotParts::Device *TwoDRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
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

	if (deviceInfo.isA<ev3::robotModel::parts::Ev3Led>()) {
		return new parts::TwoDLed(deviceInfo, port, *engine());
	}

	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

QString TwoDRobotModel::robotImage() const
{
	return ":/ev3/interpreter/images/ev3-robot.png";
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

QString TwoDRobotModel::sensorImagePath(const DeviceInfo &deviceType) const
{
	if (deviceType.isA<kitBase::robotModel::robotParts::TouchSensor>()) {
		return ":/ev3/interpreter/images/ev3-touch.png";
	} else if (deviceType.isA<kitBase::robotModel::robotParts::RangeSensor>()) {
		return ":/ev3/interpreter/images/ev3-sonar.png";
	} else {
		return QString();
	}
}


QRect TwoDRobotModel::sensorImageRect(kitBase::robotModel::DeviceInfo const &deviceType) const
{
	if (deviceType.isA<robotParts::TouchSensor>()) {
		return QRect(-12, -5, 25, 10);
	} else if (deviceType.isA<robotParts::ColorSensor>()
			|| deviceType.isA<robotParts::LightSensor>())
	{
		return QRect(-6, -6, 12, 12);
	}
	if (deviceType.isA<robotParts::RangeSensor>()) {
		return QRect(-20, -10, 40, 20);;
	} else {
		Q_ASSERT(!"Unknown sensor type");
		return QRect();
	}
}
