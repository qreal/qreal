#pragma once

#include <generatorBase/parts/deviceVariables.h>

namespace trik {
namespace parts {

/// Customizer for generator of TRIK sensor reading code.
class TrikDeviceVariables : public generatorBase::parts::DeviceVariables
{
public:
	QString variableTemplatePath(const kitBase::robotModel::DeviceInfo &device
			, const kitBase::robotModel::PortInfo &port) const override;
};

}
}
