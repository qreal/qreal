#pragma once

#include "robotModel/parts/trikLed.h"

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Led : public robotModel::parts::TrikLed
{
	Q_OBJECT

public:
	Led(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	void setColor(QString const &color) override;
};

}
}
}
}
