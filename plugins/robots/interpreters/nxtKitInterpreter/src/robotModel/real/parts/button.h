#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Button : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	Button(const interpreterBase::robotModel::DeviceInfo &info, const interpreterBase::robotModel::PortInfo &port);

	void read() override;
};

}
}
}
}
