#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

class ROBOTS_INTERPRETER_BASE_EXPORT Display : public Device
{
	Q_OBJECT
	Q_CLASSINFO("friendlyName", tr("Display"))
	Q_CLASSINFO("direction", "output")

public:
	Display(DeviceInfo const &info, PortInfo const &port);

//	virtual void drawPixel(int x, int y) = 0;
//	virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
//	virtual void drawCircle(int x, int y, int radius) = 0;
//	virtual void printText(int x, int y, QString const &text) = 0;
//	virtual void drawRect(int x, int y, int width, int height) = 0;
//	virtual void clearScreen() = 0;
};

}
}
}
