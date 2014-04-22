#include "realRobotModelV5.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include "parts/movementSensor.h"

using namespace trikKitInterpreter::robotModel::real;
using namespace interpreterBase::robotModel;

RealRobotModelV5::RealRobotModelV5()
{
	addAllowedConnection(PortInfo("JF1", input), { movementSensorInfo() });
}

QString RealRobotModelV5::name() const
{
	return "TrikRealRobotModelV5";
}

QString RealRobotModelV5::friendlyName() const
{
	return tr("Real Robot (v5)");
}

DeviceInfo RealRobotModelV5::movementSensorInfo() const
{
	return DeviceInfo::create<robotModel::parts::TrikMovementSensor>();
}

robotParts::Device *RealRobotModelV5::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA(movementSensorInfo())) {
		return new parts::MovementSensor(movementSensorInfo(), port);
	} else {
		return RealRobotModelV4::createDevice(port, deviceInfo);
	}
}
