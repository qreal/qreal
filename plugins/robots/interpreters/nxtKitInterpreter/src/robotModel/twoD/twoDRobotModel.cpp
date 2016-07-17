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
	return ":/icons/nxt_robot.png";
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
