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

#include "ev3Kit/robotModel/ev3RobotModelBase.h"

#include "ev3Kit/robotModel/parts/ev3Motor.h"
#include "ev3Kit/robotModel/parts/ev3Speaker.h"
#include "ev3Kit/robotModel/parts/ev3Display.h"
#include "ev3Kit/robotModel/parts/ev3Led.h"

#include <kitBase/robotModel/robotParts/display.h>
#include <kitBase/robotModel/robotParts/speaker.h>
#include <kitBase/robotModel/robotParts/button.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <kitBase/robotModel/robotParts/colorSensorFull.h>
#include <kitBase/robotModel/robotParts/colorSensorRed.h>
#include <kitBase/robotModel/robotParts/colorSensorGreen.h>
#include <kitBase/robotModel/robotParts/colorSensorBlue.h>
#include <kitBase/robotModel/robotParts/colorSensorPassive.h>

using namespace ev3::robotModel;
using namespace kitBase::robotModel;

Ev3RobotModelBase::Ev3RobotModelBase(const QString &kitId, const QString &robotId)
	: CommonRobotModel(kitId, robotId)
{
	QList<DeviceInfo> const inputPortConnections = {
			touchSensorInfo()
			, lightSensorInfo()
			, rangeSensorInfo()
			, colorFullSensorInfo()
			, colorRedSensorInfo()
			, colorGreenSensorInfo()
			, colorBlueSensorInfo()
			, colorPassiveSensorInfo()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });
	addAllowedConnection(PortInfo("LedPort", output), { ledInfo() });

	addAllowedConnection(PortInfo("Up", input, {}, "buttonUp"), { buttonInfo() });
	addAllowedConnection(PortInfo("Enter", input, {}, "buttonEnter"), { buttonInfo() });
	addAllowedConnection(PortInfo("Down", input, {}, "buttonDown"), { buttonInfo() });
	addAllowedConnection(PortInfo("Right", input, {}, "buttonRight"), { buttonInfo() });
	addAllowedConnection(PortInfo("Left", input, {}, "buttonLeft"), { buttonInfo() });
	addAllowedConnection(PortInfo("Back", input, {}, "buttonBack"), { buttonInfo() });

	addAllowedConnection(PortInfo("A", output), { motorInfo() });
	addAllowedConnection(PortInfo("B", output), { motorInfo() });
	addAllowedConnection(PortInfo("C", output), { motorInfo() });
	addAllowedConnection(PortInfo("D", output), { motorInfo() });

	addAllowedConnection(PortInfo("A", input, { QString::fromUtf8("А") }, "encoderA"), { encoderInfo() });
	addAllowedConnection(PortInfo("B", input, { QString::fromUtf8("В") }, "encoderB"), { encoderInfo() });
	addAllowedConnection(PortInfo("C", input, { QString::fromUtf8("С") }, "encoderC"), { encoderInfo() });
	addAllowedConnection(PortInfo("D", input, { QString::fromUtf8("D") }, "encoderD"), { encoderInfo() });

	addAllowedConnection(PortInfo("1", input, {}, "sensor1"), inputPortConnections);
	addAllowedConnection(PortInfo("2", input, {}, "sensor2"), inputPortConnections);
	addAllowedConnection(PortInfo("3", input, {}, "sensor3"), inputPortConnections);
	addAllowedConnection(PortInfo("4", input, {}, "sensor4"), inputPortConnections);
}

QList<DeviceInfo> Ev3RobotModelBase::convertibleBases() const
{
	return { DeviceInfo::create<robotParts::TouchSensor>()
			, DeviceInfo::create<robotParts::LightSensor>()
			, DeviceInfo::create<robotParts::RangeSensor>()
			, DeviceInfo::create<robotParts::ColorSensorFull>()
			, DeviceInfo::create<robotParts::ColorSensorRed>()
			, DeviceInfo::create<robotParts::ColorSensorBlue>()
			, DeviceInfo::create<robotParts::ColorSensorGreen>()
			, DeviceInfo::create<robotParts::ColorSensorPassive>()
	};
}

DeviceInfo Ev3RobotModelBase::motorInfo() const
{
	return DeviceInfo::create<ev3::robotModel::parts::Ev3Motor>();
}

DeviceInfo Ev3RobotModelBase::encoderInfo() const
{
	return DeviceInfo::create<robotParts::EncoderSensor>();
}

DeviceInfo Ev3RobotModelBase::displayInfo() const
{
	return DeviceInfo::create<ev3::robotModel::parts::Ev3Display>();
}

DeviceInfo Ev3RobotModelBase::ledInfo() const
{
	return DeviceInfo::create<ev3::robotModel::parts::Ev3Led>();
}

DeviceInfo Ev3RobotModelBase::speakerInfo() const
{
	return DeviceInfo::create<ev3::robotModel::parts::Ev3Speaker>();
}

DeviceInfo Ev3RobotModelBase::buttonInfo() const
{
	return DeviceInfo::create<robotParts::Button>();
}

DeviceInfo Ev3RobotModelBase::touchSensorInfo() const
{
	return DeviceInfo::create<robotParts::TouchSensor>();
}

DeviceInfo Ev3RobotModelBase::lightSensorInfo() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}

DeviceInfo Ev3RobotModelBase::rangeSensorInfo() const
{
	return DeviceInfo::create<robotParts::RangeSensor>();
}

DeviceInfo Ev3RobotModelBase::colorFullSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorFull>();
}

DeviceInfo Ev3RobotModelBase::colorRedSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorRed>();
}

DeviceInfo Ev3RobotModelBase::colorGreenSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorGreen>();
}

DeviceInfo Ev3RobotModelBase::colorBlueSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorBlue>();
}

DeviceInfo Ev3RobotModelBase::colorPassiveSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorPassive>();
}
