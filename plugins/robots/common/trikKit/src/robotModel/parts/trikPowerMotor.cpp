#include "trikKit/robotModel/parts/trikPowerMotor.h"

using namespace trik::robotModel::parts;
using namespace kitBase::robotModel;

TrikPowerMotor::TrikPowerMotor(const DeviceInfo &info, const PortInfo &port)
	: kitBase::robotModel::robotParts::Motor(info, port)
{
}
