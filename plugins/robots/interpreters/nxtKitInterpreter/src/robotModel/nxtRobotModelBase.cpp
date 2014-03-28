#include "nxtRobotModelBase.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/buttons.h>
#include <interpreterBase/robotModel/robotParts/motor.h>
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include <interpreterBase/robotModel/robotParts/soundSensor.h>
#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

using namespace nxtKitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

NxtRobotModelBase::NxtRobotModelBase()
{
	/// @todo Implement.
	QList<DeviceInfo> const outputPortConnections = {
		motorInfo()
		/// @todo: implement multidevice architecture
		//, encoderInfo()
	};
	QList<DeviceInfo> const inputPortConnections = {
			touchSensorInfo()
			, sonarSensorInfo()
			, lightSensorInfo()
			, colorSensorInfo()
			, soundSensorInfo()
			, gyroscopeSensorInfo()
			, accelerometerSensorInfo()
	};

	addAllowedConnection(PortInfo("DisplayPort"), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort"), { speakerInfo() });
	addAllowedConnection(PortInfo("ButtonsPort"), { buttonsInfo() });
	addAllowedConnection(PortInfo("A"), outputPortConnections);
	addAllowedConnection(PortInfo("B"), outputPortConnections);
	addAllowedConnection(PortInfo("C"), outputPortConnections);
	addAllowedConnection(PortInfo("1"), inputPortConnections);
	addAllowedConnection(PortInfo("2"), inputPortConnections);
	addAllowedConnection(PortInfo("3"), inputPortConnections);
	addAllowedConnection(PortInfo("4"), inputPortConnections);
}

QList<DeviceInfo> NxtRobotModelBase::convertibleBases() const
{
	return { DeviceInfo::create<robotParts::TouchSensor>()
			, DeviceInfo::create<robotParts::RangeSensor>()
			, DeviceInfo::create<robotParts::LightSensor>()
			, DeviceInfo::create<robotParts::ColorSensor>()
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

DeviceInfo NxtRobotModelBase::buttonsInfo() const
{
	return DeviceInfo::create<robotParts::Buttons>();
}

DeviceInfo NxtRobotModelBase::motorInfo() const
{
	return DeviceInfo::create<robotParts::Motor>();
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

DeviceInfo NxtRobotModelBase::colorSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensor>();
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
