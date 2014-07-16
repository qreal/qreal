#pragma once

#include <interpreterBase/robotModel/robotParts/display.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace parts {

class NxtDisplay : public interpreterBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	NxtDisplay(interpreterBase::robotModel::DeviceInfo const &info, interpreterBase::robotModel::PortInfo const &port);

	//virtual void drawPixel(int x, int y) = 0;
	//virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
	//virtual void drawRect(int x, int y, int width, int height) = 0;
	virtual void drawCircle(int x, int y, int radius) = 0;
};

}
}
}
