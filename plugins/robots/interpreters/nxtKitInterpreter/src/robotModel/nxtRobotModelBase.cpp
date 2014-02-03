#include "nxtRobotModelBase.h"
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/sonarSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include <interpreterBase/robotModel/robotParts/soundSensor.h>
#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

using namespace nxtKitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

NxtRobotModelBase::NxtRobotModelBase()
{
	QList<PluggableDeviceInfo> const outputPortConnections; // TODO
	QList<PluggableDeviceInfo> const inputPortConnections = QList<PluggableDeviceInfo>()
			<< touchSensorInfo()
			<< sonarSensorInfo()
			<< lightSensorInfo()
			<< colorSensorInfo()
			<< soundSensorInfo()
			<< gyroscopeSensorInfo()
			<< accelerometerSensorInfo();
	mAllowedConnections[PortInfo("A")] = outputPortConnections;
	mAllowedConnections[PortInfo("B")] = outputPortConnections;
	mAllowedConnections[PortInfo("C")] = outputPortConnections;
	mAllowedConnections[PortInfo("1")] = inputPortConnections;
	mAllowedConnections[PortInfo("2")] = inputPortConnections;
	mAllowedConnections[PortInfo("3")] = inputPortConnections;
	mAllowedConnections[PortInfo("4")] = inputPortConnections;
}

QList<PortInfo> NxtRobotModelBase::configurablePorts() const
{
	return QList<PortInfo>()
			<< PortInfo("1")
			<< PortInfo("2")
			<< PortInfo("3")
			<< PortInfo("4");
}

PluggableDeviceInfo NxtRobotModelBase::touchSensorInfo() const
{
	return PluggableDeviceInfo::create<robotParts::TouchSensor>(tr("Touch sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::sonarSensorInfo() const
{
	return PluggableDeviceInfo::create<robotParts::SonarSensor>(tr("Sonar sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::lightSensorInfo() const
{
	return PluggableDeviceInfo::create<robotParts::LightSensor>(tr("Light sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::colorSensorInfo() const
{
	return PluggableDeviceInfo::create<robotParts::ColorSensor>(tr("Color sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::soundSensorInfo() const
{
	return PluggableDeviceInfo::create<robotParts::SoundSensor>(tr("Sound sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::gyroscopeSensorInfo() const
{
	return PluggableDeviceInfo::create<robotParts::GyroscopeSensor>(tr("Gyroscope"));
}

PluggableDeviceInfo NxtRobotModelBase::accelerometerSensorInfo() const
{
	return PluggableDeviceInfo::create<robotParts::AccelerometerSensor>(tr("Accelerometer"));
}
