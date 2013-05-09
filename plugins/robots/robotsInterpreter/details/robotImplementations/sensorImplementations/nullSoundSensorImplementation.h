
#include <QtCore/QTimer>
#include "abstractSoundImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class NullEncoderImplementation : public AbstractSoundImplementation
{
	Q_OBJECT
public:
	NullSoundImplementation(outputPort::OutputPortEnum const &port);
	virtual void read();
	virtual void nullificate();

protected slots:
	virtual void timerTimeout();

private:
	QTimer mActiveWaitingTimer;
};

}
}
}
}
}
}
