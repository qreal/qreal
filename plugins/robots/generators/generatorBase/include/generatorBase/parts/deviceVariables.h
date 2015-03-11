#pragma once

#include <kitBase/robotModel/deviceInfo.h>
#include <kitBase/robotModel/portInfo.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace parts {

/// Base class that allows customization of device related code generation. Supposed to be subclassed in generator kit
/// plugins.
class ROBOTS_GENERATOR_EXPORT DeviceVariables
{
public:
	virtual ~DeviceVariables();

	/// Allows to customize path to device expression template based on device info and port info.
	virtual QString variableTemplatePath(const kitBase::robotModel::DeviceInfo &device
			, const kitBase::robotModel::PortInfo &port) const;
};

}
}
