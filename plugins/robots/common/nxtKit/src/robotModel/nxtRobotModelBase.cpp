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

#include "nxtKit/robotModel/nxtRobotModelBase.h"

#include <kitBase/robotModel/robotParts/display.h>
#include <kitBase/robotModel/robotParts/speaker.h>
#include <kitBase/robotModel/robotParts/button.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>

#include "nxtKit/robotModel/parts/nxtMotor.h"

#include <kitBase/robotModel/robotParts/colorSensorFull.h>
#include <kitBase/robotModel/robotParts/colorSensorRed.h>
#include <kitBase/robotModel/robotParts/colorSensorGreen.h>
#include <kitBase/robotModel/robotParts/colorSensorBlue.h>
#include <kitBase/robotModel/robotParts/colorSensorPassive.h>
#include <kitBase/robotModel/robotParts/soundSensor.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include <kitBase/robotModel/robotParts/accelerometerSensor.h>

using namespace nxt::robotModel;
using namespace kitBase::robotModel;

NxtRobotModelBase::NxtRobotModelBase(const QString &kitId, const QString &robotId)
	: CommonRobotModel(kitId, robotId)
{
	QList<DeviceInfo> const inputPortConnections = {
			touchSensorInfo()
			, sonarSensorInfo()
			, lightSensorInfo()
			, colorFullSensorInfo()
			, colorRedSensorInfo()
			, colorGreenSensorInfo()
			, colorBlueSensorInfo()
			, colorPassiveSensorInfo()
			, soundSensorInfo()
			, gyroscopeSensorInfo()
			, accelerometerSensorInfo()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });

	addAllowedConnection(PortInfo("Left", input, {}, "buttonLeft"), { buttonInfo() });
	addAllowedConnection(PortInfo("Right", input, {}, "buttonRight"), { buttonInfo() });
	addAllowedConnection(PortInfo("Enter", input, {}, "buttonEnter"), { buttonInfo() });
	addAllowedConnection(PortInfo("Escape", input, {}, "buttonEscape"), { buttonInfo() });

	addAllowedConnection(PortInfo("A", output, { QString::fromUtf8("А") }), { motorInfo() });
	addAllowedConnection(PortInfo("B", output, { QString::fromUtf8("В") }), { motorInfo() });
	addAllowedConnection(PortInfo("C", output, { QString::fromUtf8("С") }), { motorInfo() });

	addAllowedConnection(PortInfo("A", input, { QString::fromUtf8("А") }, "encoderA"), { encoderInfo() });
	addAllowedConnection(PortInfo("B", input, { QString::fromUtf8("В") }, "encoderB"), { encoderInfo() });
	addAllowedConnection(PortInfo("C", input, { QString::fromUtf8("С") }, "encoderC"), { encoderInfo() });

	addAllowedConnection(PortInfo("1", input, {}, "sensor1"), inputPortConnections);
	addAllowedConnection(PortInfo("2", input, {}, "sensor2"), inputPortConnections);
	addAllowedConnection(PortInfo("3", input, {}, "sensor3"), inputPortConnections);
	addAllowedConnection(PortInfo("4", input, {}, "sensor4"), inputPortConnections);
}

QList<DeviceInfo> NxtRobotModelBase::convertibleBases() const
{
	return { DeviceInfo::create<robotParts::TouchSensor>()
			, DeviceInfo::create<robotParts::RangeSensor>()
			, DeviceInfo::create<robotParts::LightSensor>()
			, DeviceInfo::create<robotParts::ColorSensorFull>()
			, DeviceInfo::create<robotParts::ColorSensorRed>()
			, DeviceInfo::create<robotParts::ColorSensorGreen>()
			, DeviceInfo::create<robotParts::ColorSensorBlue>()
			, DeviceInfo::create<robotParts::ColorSensorPassive>()
			, DeviceInfo::create<robotParts::SoundSensor>()
			, DeviceInfo::create<robotParts::GyroscopeSensor>()
			, DeviceInfo::create<robotParts::AccelerometerSensor>()
	};
}

DeviceInfo NxtRobotModelBase::displayInfo() const
{
	return DeviceInfo::create<robotParts::Display>();
}

DeviceInfo NxtRobotModelBase::speakerInfo() const
{
	return DeviceInfo::create<robotParts::Speaker>();
}

DeviceInfo NxtRobotModelBase::buttonInfo() const
{
	return DeviceInfo::create<robotParts::Button>();
}

DeviceInfo NxtRobotModelBase::motorInfo() const
{
	return DeviceInfo::create<nxt::robotModel::parts::NxtMotor>();
}

DeviceInfo NxtRobotModelBase::encoderInfo() const
{
	return DeviceInfo::create<robotParts::EncoderSensor>();
}

DeviceInfo NxtRobotModelBase::touchSensorInfo() const
{
	return DeviceInfo::create<robotParts::TouchSensor>();
}

DeviceInfo NxtRobotModelBase::sonarSensorInfo() const
{
	return DeviceInfo::create<robotParts::RangeSensor>();
}

DeviceInfo NxtRobotModelBase::lightSensorInfo() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}

DeviceInfo NxtRobotModelBase::colorFullSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorFull>();
}

DeviceInfo NxtRobotModelBase::colorRedSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorRed>();
}

DeviceInfo NxtRobotModelBase::colorGreenSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorGreen>();
}

DeviceInfo NxtRobotModelBase::colorBlueSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorBlue>();
}

DeviceInfo NxtRobotModelBase::colorPassiveSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensorPassive>();
}

DeviceInfo NxtRobotModelBase::soundSensorInfo() const
{
	return DeviceInfo::create<robotParts::SoundSensor>();
}

DeviceInfo NxtRobotModelBase::gyroscopeSensorInfo() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>();
}

DeviceInfo NxtRobotModelBase::accelerometerSensorInfo() const
{
	return DeviceInfo::create<robotParts::AccelerometerSensor>();
}
