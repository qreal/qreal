#include "../robotImplementations/sensorImplementations/abstractDisplayImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class Display : public QObject
{
	Q_OBJECT
public:
	Display(robotImplementations::sensorImplementations::AbstractDisplayImplementation *displayImpl);
	virtual ~Display();
	virtual void read();
	robotImplementations::sensorImplementations::AbstractDisplayImplementation *displayImpl();
	void setImplementation(robotImplementations::sensorImplementations::AbstractDisplayImplementation *displayImpl);

protected:
	robotImplementations::sensorImplementations::AbstractDisplayImplementation *mDisplayImpl;
};

}
}
}
}
}
