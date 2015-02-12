#include "trikServoMotor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikServoMotor::TrikServoMotor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
{
}
