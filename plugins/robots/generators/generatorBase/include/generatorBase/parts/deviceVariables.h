#pragma once

#include <interpreterBase/robotModel/deviceInfo.h>
#include <interpreterBase/robotModel/portInfo.h>

#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace parts {

class ROBOTS_GENERATOR_EXPORT DeviceVariables
{
public:
	virtual ~DeviceVariables();

	virtual QString variableTemplatePath(interpreterBase::robotModel::DeviceInfo const &device
			, interpreterBase::robotModel::PortInfo const &port) const;
};

}
}
