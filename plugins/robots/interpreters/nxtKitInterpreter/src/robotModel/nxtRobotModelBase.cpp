#include "nxtRobotModelBase.h"

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
		<< PluggableDeviceInfo("touchBool", tr("Touch sensor (boolean value)"))
		<< PluggableDeviceInfo("touchRaw", tr("Touch sensor (raw value)"))
		<< PluggableDeviceInfo("sonar", tr("Sonar sensor"))
		<< PluggableDeviceInfo("loght", tr("Light sensor"))
		<< PluggableDeviceInfo("colorFull", tr("Color sensor (full colors)"))
		<< PluggableDeviceInfo("colorRed", tr("Color sensor (red)"))
		<< PluggableDeviceInfo("colorGreen", tr("Color sensor (green)"))
		<< PluggableDeviceInfo("colorBlue", tr("Color sensor (blue)"))
		<< PluggableDeviceInfo("colorPassive", tr("Color sensor (passive)"))
		<< PluggableDeviceInfo("sound", tr("Sound sensor"))
		<< PluggableDeviceInfo("gyroscope", tr("Gyroscope"));
		// << PluggableDeviceInfo("accelerometer", tr("Accelerometer"));
}
