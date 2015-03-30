#include "generatorBase/parts/deviceVariables.h"

#include <kitBase/robotModel/robotParts/button.h>

using namespace generatorBase::parts;

DeviceVariables::~DeviceVariables()
{
}

QString DeviceVariables::variableTemplatePath(const kitBase::robotModel::DeviceInfo &device
		, const kitBase::robotModel::PortInfo &port) const
{
	/// @todo Remove buttons from here.
	return QString("sensors/%1.t").arg(
			device.isA<kitBase::robotModel::robotParts::Button>()
					? port.name()
					: device.name());
}
