#pragma once

#include "abstractDisplayImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace displayImplementations {

class NullDisplayImplementation : public AbstractDisplayImplementation
{
	Q_OBJECT

public:
	NullDisplayImplementation();
	virtual ~NullDisplayImplementation() {}

	virtual void read();
	virtual void drawPixel(int x, int y);
	virtual void drawLine(int x1, int y1, int x2, int y2);
	virtual void drawRect(int x, int y, int width, int height);
	virtual void drawCircle(int x, int y, int radius);
	virtual void printText(int x, int y, QString const &text);
	virtual void clearScreen();

signals:
	void response(bool leftIsPressed, bool rightIsPressed
			, bool centralIsPressed, bool bottomIsPressed);
};

}
}
}
}
}
}
