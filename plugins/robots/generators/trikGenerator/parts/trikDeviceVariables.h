#pragma once

#include <generatorBase/parts/deviceVariables.h>

namespace trik {
namespace parts {

class TrikDeviceVariables : public generatorBase::parts::DeviceVariables
{
public:
	QString variableTemplatePath(interpreterBase::robotModel::DeviceInfo const &device
			, interpreterBase::robotModel::PortInfo const &port) const;
};

}
}
