#include "trikPowerMotor.h"

using namespace trikKitInterpreter::robotModel::parts;
using namespace interpreterBase::robotModel;

TrikPowerMotor::TrikPowerMotor(DeviceInfo const &info, PortInfo const &port)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
{
}

void TrikPowerMotor::doConfiguration()
{
	configurationCompleted(true);
}
