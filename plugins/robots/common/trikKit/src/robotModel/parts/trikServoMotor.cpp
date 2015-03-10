#include "trikKit/robotModel/parts/trikServoMotor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikServoMotor::TrikServoMotor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Motor(info, port)
{
}
