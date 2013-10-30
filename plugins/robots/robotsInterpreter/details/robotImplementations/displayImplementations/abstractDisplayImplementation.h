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
	virtual void drawImage(QImage const &image) = 0;
	virtual void clearScreen() = 0;

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
