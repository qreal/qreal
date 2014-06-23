#include "powerMotor.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

PowerMotor::PowerMotor(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikPowerMotor(info, port)
{
}
