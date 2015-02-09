#include "generatorBase/parts/deviceVariables.h"

#include <interpreterBase/robotModel/robotParts/button.h>

using namespace generatorBase::parts;

DeviceVariables::~DeviceVariables()
{
}

QString DeviceVariables::variableTemplatePath(interpreterBase::robotModel::DeviceInfo const &device
		, interpreterBase::robotModel::PortInfo const &port) const
{
	/// @todo Remove buttons from here.
	return QString("sensors/%1.t").arg(
			device.isA<interpreterBase::robotModel::robotParts::Button>()
					? port.name()
					: device.name());
}
