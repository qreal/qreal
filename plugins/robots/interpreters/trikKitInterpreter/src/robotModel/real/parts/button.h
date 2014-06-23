#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Button : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}
