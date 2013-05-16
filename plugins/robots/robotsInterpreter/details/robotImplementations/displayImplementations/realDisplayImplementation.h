#pragma once

#include "abstractDisplayImplementation.h"
#include "../../d2RobotModel/d2RobotModel.h"
#include "../details/nxtDisplay.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace displayImplementations {

class RealDisplayImplementation : public AbstractDisplayImplementation
{
	Q_OBJECT
public:
	RealDisplayImplementation();
	virtual ~RealDisplayImplementation() {}
	virtual void read();
	virtual void drawPixel(int x, int y);
	virtual void drawLine(int x1, int y1, int x2, int y2);
	virtual void drawRect(int x, int y, int width, int height);
	virtual void drawCircle(int x, int y, int radius);
	virtual void printText(int x, int y, QString text);
	virtual void clearScreen();
	virtual void attachToPaintWidget();

signals:
	void response(bool leftIsPressed
				  , bool rightIsPressed
				  , bool centralIsPressed
				  , bool bottomIsPressed);
};

}
}
}
}
}
}


