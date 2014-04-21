#pragma once

#include <interpreterBase/robotModel/robotParts/buttons.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikButtons : public interpreterBase::robotModel::robotParts::Buttons
{
	Q_OBJECT

public:
	TrikButtons(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

signals:
	void response(bool leftIsPressed, bool rightIsPressed, bool centralIsPressed, bool bottomIsPressed);

private:
	void doConfiguration() override;
};

}
}
}
