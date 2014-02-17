#include "nxtRobotModelBase.h"

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
	QList<DeviceInfo> const outputPortConnections;
	QList<DeviceInfo> const inputPortConnections = QList<DeviceInfo>()
			<< touchSensorInfo()
			<< sonarSensorInfo()
			<< lightSensorInfo()
			<< colorSensorInfo()
			<< soundSensorInfo()
			<< gyroscopeSensorInfo()
			<< accelerometerSensorInfo();

	addAllowedConnection(PortInfo("A"), outputPortConnections);
	addAllowedConnection(PortInfo("A"), outputPortConnections);
	addAllowedConnection(PortInfo("B"), outputPortConnections);
	addAllowedConnection(PortInfo("C"), outputPortConnections);
	addAllowedConnection(PortInfo("1"), inputPortConnections);
	addAllowedConnection(PortInfo("2"), inputPortConnections);
	addAllowedConnection(PortInfo("3"), inputPortConnections);
	addAllowedConnection(PortInfo("4"), inputPortConnections);
}

QList<PortInfo> NxtRobotModelBase::configurablePorts() const
{
	return QList<PortInfo>()
			<< PortInfo("1")
			<< PortInfo("2")
			<< PortInfo("3")
			<< PortInfo("4");
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

DeviceInfo NxtRobotModelBase::touchSensorInfo() const
{
	return DeviceInfo::create<robotParts::TouchSensor>(tr("Touch sensor"));
}

DeviceInfo NxtRobotModelBase::sonarSensorInfo() const
{
	return DeviceInfo::create<robotParts::RangeSensor>(tr("Sonar sensor"));
}

DeviceInfo NxtRobotModelBase::lightSensorInfo() const
{
	return DeviceInfo::create<robotParts::LightSensor>(tr("Light sensor"));
}

DeviceInfo NxtRobotModelBase::colorSensorInfo() const
{
	return DeviceInfo::create<robotParts::ColorSensor>(tr("Color sensor"));
}

DeviceInfo NxtRobotModelBase::soundSensorInfo() const
{
	return DeviceInfo::create<robotParts::SoundSensor>(tr("Sound sensor"));
}

DeviceInfo NxtRobotModelBase::gyroscopeSensorInfo() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>(tr("Gyroscope"));
}

DeviceInfo NxtRobotModelBase::accelerometerSensorInfo() const
{
	return DeviceInfo::create<robotParts::AccelerometerSensor>(tr("Accelerometer"));
}
