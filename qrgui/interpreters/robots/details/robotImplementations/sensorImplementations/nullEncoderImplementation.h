#pragma once

#include <QtCore/QTimer>
#include "AbstractEncoderImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class NullEncoderImplementation : public AbstractEncoderImplementation
{
	Q_OBJECT
public:
	NullEncoderImplementation(outputPort::OutputPortEnum const &port);
	virtual void read();

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
