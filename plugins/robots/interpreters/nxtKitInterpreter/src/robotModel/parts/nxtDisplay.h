#pragma once

#include <interpreterBase/robotModel/robotParts/display.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace parts {

class NxtDisplay : public interpreterBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	NxtDisplay(const interpreterBase::robotModel::DeviceInfo &info, const interpreterBase::robotModel::PortInfo &port);

	virtual void drawCircle(int x, int y, int radius) = 0;
};

}
}
}
