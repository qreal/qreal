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
	Display(robotImplementations::displayImplementations::AbstractDisplayImplementation *displayImpl);
	virtual ~Display();
	virtual void read();
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
