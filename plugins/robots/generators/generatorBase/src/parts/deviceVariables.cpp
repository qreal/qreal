#include "generatorBase/parts/deviceVariables.h"

#include <interpreterBase/robotModel/robotParts/button.h>

using namespace generatorBase::parts;

DeviceVariables::~DeviceVariables()
{
}

QString DeviceVariables::variableTemplatePath(const interpreterBase::robotModel::DeviceInfo &device
		, const interpreterBase::robotModel::PortInfo &port) const
{
	/// @todo Remove buttons from here.
	return QString("sensors/%1.t").arg(
			device.isA<interpreterBase::robotModel::robotParts::Button>()
					? port.name().split("ButtonPort", QString::SkipEmptyParts)[0]
					: device.name());
}
