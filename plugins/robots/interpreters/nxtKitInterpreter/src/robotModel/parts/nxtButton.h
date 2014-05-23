#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace parts {

class NxtButton : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	NxtButton(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

signals:
	void response(bool leftIsPressed, bool rightIsPressed, bool centralIsPressed, bool bottomIsPressed);

private:
	void doConfiguration() override;
};

}
}
}
