#pragma once

#include "robotModel/parts/nxtButtons.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Buttons : public robotModel::parts::NxtButtons
{
	Q_OBJECT

public:
	Buttons(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

	void read() override;
};

}
}
}
}
