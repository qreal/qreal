#include "realRobotModelV6.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include "parts/movementSensor.h"

using namespace trikKitInterpreter::robotModel::real;
using namespace interpreterBase::robotModel;

RealRobotModelV6::RealRobotModelV6(QString const &kitId)
	: RealRobotModelBase(kitId)
{
	addAllowedConnection(PortInfo("JM4", output), { powerMotorInfo() });
	addAllowedConnection(PortInfo("JF1", input), { movementSensorInfo() });

	addAllowedConnection(PortInfo("JB3", input, { "JM3" }, "encoder3"), { encoderInfo() });
	addAllowedConnection(PortInfo("JB4", input, { "JM4" }, "encoder4"), { encoderInfo() });
}

QString RealRobotModelV6::name() const
{
	return "TrikRealRobotModelV6";
}

QString RealRobotModelV6::friendlyName() const
{
	return tr("Real Robot (v6)");
}

DeviceInfo RealRobotModelV6::movementSensorInfo() const
{
	return DeviceInfo::create<robotModel::parts::TrikMovementSensor>();
}

robotParts::Device *RealRobotModelV6::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA(movementSensorInfo())) {
		return new parts::MovementSensor(movementSensorInfo(), port);
	} else {
		return RealRobotModelBase::createDevice(port, deviceInfo);
	}
}
