#pragma once

#include "robotModel/parts/trikButtons.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Buttons : public robotModel::parts::TrikButtons
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
