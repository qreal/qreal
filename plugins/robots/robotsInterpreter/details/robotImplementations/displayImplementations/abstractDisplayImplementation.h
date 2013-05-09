#pragma once
#include <QtCore/QObject>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace displayImplementations {


class AbstractDisplayImplementation : public QObject
{
	Q_OBJECT
public:
	AbstractDisplayImplementation();
	virtual ~AbstractDisplayImplementation() {}
	virtual void read() = 0;
	virtual void drawPixel(int x, int y) = 0;
	virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
	virtual void drawCircle(int x, int y, int radius) = 0;
	virtual void printText(int x, int y, QString text) = 0;

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
