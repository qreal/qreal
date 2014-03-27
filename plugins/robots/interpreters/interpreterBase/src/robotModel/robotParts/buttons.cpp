#include "interpreterBase/robotModel/robotParts/buttons.h"

using namespace interpreterBase::robotModel::robotParts;

QString Buttons::friendlyName()
{
	return tr("Buttons");
}

interpreterBase::robotModel::DeviceInfo::Direction Buttons::direction()
{
	return DeviceInfo::input;
}

Buttons::Buttons(DeviceInfo const &info, PortInfo const &port)
	: Device(info, port)
{
}
