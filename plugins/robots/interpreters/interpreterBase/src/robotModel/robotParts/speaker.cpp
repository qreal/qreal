#include "interpreterBase/robotModel/robotParts/speaker.h"

using namespace interpreterBase::robotModel;
using namespace robotParts;

QString Speaker::friendlyName()
{
	return tr("Speaker");
}

DeviceInfo::Direction Speaker::direction()
{
	return DeviceInfo::output;
}

Speaker::Speaker(DeviceInfo const &info, PortInfo const &port)
	: Device(info, port)
{
}
