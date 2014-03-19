#pragma once

#include <interpreterBase/robotModel/robotParts/display.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Display : public interpreterBase::robotModel::robotParts::Display
{
public:
	Display(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

private:
	void doConfiguration() override;
};

}
}
}
}
