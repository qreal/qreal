#include "nxtRobotModelBase.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/button.h>
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>

#include "parts/nxtMotor.h"

/// @todo: use common sensors
#include "real/parts/colorSensorFull.h"
#include "real/parts/colorSensorRed.h"
#include "real/parts/colorSensorGreen.h"
#include "real/parts/colorSensorBlue.h"
#include "real/parts/colorSensorPassive.h"
#include <interpreterBase/robotModel/robotParts/soundSensor.h>
#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

using namespace nxtKitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

NxtRobotModelBase::NxtRobotModelBase()
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

	addAllowedConnection(PortInfo("LeftButtonPort", input, {}, "leftButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("RightButtonPort", input, {}, "rightButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("EnterButtonPort", input, {}, "enterButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("EscapeButtonPort", input, {}, "escapeButton"), { buttonInfo() });

	addAllowedConnection(PortInfo("A", output), { motorInfo() });
	addAllowedConnection(PortInfo("B", output), { motorInfo() });
	addAllowedConnection(PortInfo("C", output), { motorInfo() });

	addAllowedConnection(PortInfo("A", input, {}, "encoderA"), { encoderInfo() });
	addAllowedConnection(PortInfo("B", input, {}, "encoderB"), { encoderInfo() });
	addAllowedConnection(PortInfo("C", input, {}, "encoderC"), { encoderInfo() });

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
			, DeviceInfo::create<robotModel::real::parts::ColorSensorFull>()
			, DeviceInfo::create<robotModel::real::parts::ColorSensorRed>()
			, DeviceInfo::create<robotModel::real::parts::ColorSensorGreen>()
			, DeviceInfo::create<robotModel::real::parts::ColorSensorBlue>()
			, DeviceInfo::create<robotModel::real::parts::ColorSensorPassive>()
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
	return DeviceInfo::create<nxtKitInterpreter::robotModel::parts::NxtMotor>();
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
	return DeviceInfo::create<robotModel::real::parts::ColorSensorFull>();
}

DeviceInfo NxtRobotModelBase::colorRedSensorInfo() const
{
	return DeviceInfo::create<robotModel::real::parts::ColorSensorRed>();
}

DeviceInfo NxtRobotModelBase::colorGreenSensorInfo() const
{
	return DeviceInfo::create<robotModel::real::parts::ColorSensorGreen>();
}

DeviceInfo NxtRobotModelBase::colorBlueSensorInfo() const
{
	return DeviceInfo::create<robotModel::real::parts::ColorSensorBlue>();
}

DeviceInfo NxtRobotModelBase::colorPassiveSensorInfo() const
{
	return DeviceInfo::create<robotModel::real::parts::ColorSensorPassive>();
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
