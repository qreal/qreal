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

QList<PortInfo> NxtRobotModelBase::availablePorts() const
{
	return QList<PortInfo>()
			<< PortInfo("A", PortInfo::inOut)
			<< PortInfo("B", PortInfo::inOut)
			<< PortInfo("C", PortInfo::inOut)
			<< PortInfo("1", PortInfo::input)
			<< PortInfo("2", PortInfo::input)
			<< PortInfo("3", PortInfo::input)
			<< PortInfo("4", PortInfo::input);
}

QList<PluggableDeviceInfo> NxtRobotModelBase::supportedSensors() const
{
	return QList<PluggableDeviceInfo>()
			<< touchSensorInfo()
			<< sonarSensorInfo()
			<< lightSensorInfo()
			<< colorSensorInfo()
			<< soundSensorInfo()
			<< gyroscopeSensorInfo()
			<< accelerometerSensorInfo();
//		<< PluggableDeviceInfo("nxtKit", "touchBool", tr("Touch sensor (boolean value)"))
//		<< PluggableDeviceInfo("nxtKit", "touchRaw", tr("Touch sensor (raw value)"))
//		<< PluggableDeviceInfo("nxtKit", "sonar", tr("Sonar sensor"))
//		<< PluggableDeviceInfo("nxtKit", "light", tr("Light sensor"))
//		<< PluggableDeviceInfo("nxtKit", "colorFull", tr("Color sensor (full colors)"))
//		<< PluggableDeviceInfo("nxtKit", "colorRed", tr("Color sensor (red)"))
//		<< PluggableDeviceInfo("nxtKit", "colorGreen", tr("Color sensor (green)"))
//		<< PluggableDeviceInfo("nxtKit", "colorBlue", tr("Color sensor (blue)"))
//		<< PluggableDeviceInfo("nxtKit", "colorPassive", tr("Color sensor (passive)"))
//		<< PluggableDeviceInfo("nxtKit", "sound", tr("Sound sensor"))
//		<< PluggableDeviceInfo("nxtKit", "gyroscope", tr("Gyroscope"));
	// << PluggableDeviceInfo("accelerometer", tr("Accelerometer"));
}

PluggableDeviceInfo NxtRobotModelBase::touchSensorInfo() const
{
	return PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::ColorSensor>(tr("Touch sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::sonarSensorInfo() const
{
	return PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::RangeSensor>(tr("Sonar sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::lightSensorInfo() const
{
	return PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::LightSensor>(tr("Light sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::colorSensorInfo() const
{
	return PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::ColorSensor>(tr("Color sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::soundSensorInfo() const
{
	return PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::SoundSensor>(tr("Sound sensor"));
}

PluggableDeviceInfo NxtRobotModelBase::gyroscopeSensorInfo() const
{
	return PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::GyroscopeSensor>(tr("Gyroscope"));
}

PluggableDeviceInfo NxtRobotModelBase::accelerometerSensorInfo() const
{
	return PluggableDeviceInfo::create<interpreterBase::robotModel::robotParts::AccelerometerSensor>(
			tr("Accelerometer"));
}
