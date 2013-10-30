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
