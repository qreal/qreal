#pragma once

#include <interpreterBase/robotModel/robotParts/button.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikButton : public interpreterBase::robotModel::robotParts::Button
{
	Q_OBJECT

public:
	TrikButton(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

signals:
	void response(bool leftIsPressed
			, bool rightIsPressed
			, bool upIsPressed
			, bool downIsPressed
			, bool enterIsPresed
			, bool escapeIsPresed
			, bool powerIsPressed);

private:
	void doConfiguration() override;
};

}
}
}
