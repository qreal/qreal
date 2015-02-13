#pragma once

#include "robotModel/parts/nxtDisplay.h"

namespace nxtKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Display : public robotModel::parts::NxtDisplay
{
	Q_OBJECT

public:
	Display(const interpreterBase::robotModel::DeviceInfo &info, const interpreterBase::robotModel::PortInfo &port);

	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height) override;
	void drawCircle(int x, int y, int radius) override;
	void printText(int x, int y, const QString &text) override;
	void clearScreen() override;
};

}
}
}
}
