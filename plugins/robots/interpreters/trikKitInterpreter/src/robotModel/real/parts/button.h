#pragma once

#include "robotModel/parts/trikButton.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Button : public robotModel::parts::TrikButton
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
