#pragma once

#include "robotModel/parts/nxtButton.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Button : public robotModel::parts::NxtButton
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
