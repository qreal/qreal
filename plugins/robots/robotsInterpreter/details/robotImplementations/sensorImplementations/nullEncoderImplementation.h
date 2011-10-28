#pragma once

#include <QtCore/QTimer>
#include "abstractEncoderImplementation.h"

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
