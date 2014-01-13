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
	// TODO: use kit name from common place
	return QList<PluggableDeviceInfo>()
		<< PluggableDeviceInfo("nxtKit", "touchBool", tr("Touch sensor (boolean value)"))
		<< PluggableDeviceInfo("nxtKit", "touchRaw", tr("Touch sensor (raw value)"))
		<< PluggableDeviceInfo("nxtKit", "sonar", tr("Sonar sensor"))
		<< PluggableDeviceInfo("nxtKit", "light", tr("Light sensor"))
		<< PluggableDeviceInfo("nxtKit", "colorFull", tr("Color sensor (full colors)"))
		<< PluggableDeviceInfo("nxtKit", "colorRed", tr("Color sensor (red)"))
		<< PluggableDeviceInfo("nxtKit", "colorGreen", tr("Color sensor (green)"))
		<< PluggableDeviceInfo("nxtKit", "colorBlue", tr("Color sensor (blue)"))
		<< PluggableDeviceInfo("nxtKit", "colorPassive", tr("Color sensor (passive)"))
		<< PluggableDeviceInfo("nxtKit", "sound", tr("Sound sensor"))
		<< PluggableDeviceInfo("nxtKit", "gyroscope", tr("Gyroscope"));
		// << PluggableDeviceInfo("accelerometer", tr("Accelerometer"));
}
