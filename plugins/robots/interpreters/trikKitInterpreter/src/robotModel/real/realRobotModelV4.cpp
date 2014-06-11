#include "realRobotModelV4.h"

using namespace trikKitInterpreter::robotModel::real;
using namespace interpreterBase::robotModel;

RealRobotModelV4::RealRobotModelV4(QString const &kitId)
	: RealRobotModelBase(kitId)
{
	addAllowedConnection(PortInfo("M1", output), { powerMotorInfo() });

	addAllowedConnection(PortInfo("JB3", input, { "M1" }, "encoder3"), { encoderInfo() });
	addAllowedConnection(PortInfo("JB4", input, { "JM3" }, "encoder4"), { encoderInfo() });
}

QString RealRobotModelV4::name() const
{
	return "TrikRealRobotModelV4";
}

QString RealRobotModelV4::friendlyName() const
{
	return tr("Real Robot (v4)");
}
