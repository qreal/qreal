#include "trikPowerMotor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikPowerMotor::TrikPowerMotor(const DeviceInfo &info, const PortInfo &port)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
{
}
