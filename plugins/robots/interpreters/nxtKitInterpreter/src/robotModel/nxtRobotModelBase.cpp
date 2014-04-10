#include "nxtRobotModelBase.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/buttons.h>
#include <interpreterBase/robotModel/robotParts/motor.h>
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>
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
	/// @todo Implement.
	QList<DeviceInfo> const outputPortConnections = {
		motorInfo()
		, encoderInfo()
	};
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

	addAllowedConnection(PortInfo("DisplayPort"), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort"), { speakerInfo() });
	addAllowedConnection(PortInfo("ButtonsPort"), { buttonsInfo() });
	addAllowedConnection(PortInfo("A", {}, "encoderA"), outputPortConnections);
	addAllowedConnection(PortInfo("B", {}, "encoderB"), outputPortConnections);
	addAllowedConnection(PortInfo("C", {}, "encoderC"), outputPortConnections);
	addAllowedConnection(PortInfo("1", {}, "sensor1"), inputPortConnections);
	addAllowedConnection(PortInfo("2", {}, "sensor2"), inputPortConnections);
	addAllowedConnection(PortInfo("3", {}, "sensor3"), inputPortConnections);
	addAllowedConnection(PortInfo("4", {}, "sensor4"), inputPortConnections);
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
