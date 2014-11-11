#pragma once

#include <interpreterBase/robotModel/deviceInfo.h>
#include <interpreterBase/robotModel/portInfo.h>

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
	virtual QString variableTemplatePath(interpreterBase::robotModel::DeviceInfo const &device
			, interpreterBase::robotModel::PortInfo const &port) const;
};

}
}
