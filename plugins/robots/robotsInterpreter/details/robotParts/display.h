#pragma once

#include "../robotImplementations/displayImplementations/abstractDisplayImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Display : public QObject
{
	Q_OBJECT

public:
	explicit Display(robotImplementations::displayImplementations::AbstractDisplayImplementation *displayImpl);
	virtual ~Display();

	virtual void read();
	virtual void drawPixel(int x, int y);
	virtual void drawLine(int x1, int y1, int x2, int y2);
	virtual void drawCircle(int x, int y, int radius);
	virtual void printText(int x, int y, QString const &text);
	virtual void drawRect(int x, int y, int width, int height);
	virtual void clearScreen();
	robotImplementations::displayImplementations::AbstractDisplayImplementation *displayImpl();
	void setImplementation(robotImplementations::displayImplementations::AbstractDisplayImplementation *displayImpl);

protected:
	robotImplementations::displayImplementations::AbstractDisplayImplementation *mDisplayImpl;
};

}
}
}
}
}
