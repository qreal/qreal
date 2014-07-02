#include "trikServoMotor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikServoMotor::TrikServoMotor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
{
}
