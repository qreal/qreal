#include "servoMotor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

ServoMotor::ServoMotor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikServoMotor(info, port)
{
}
