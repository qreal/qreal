#pragma once

#include <QtCore/QObject>

#include "../robotImplementations/sensorImplementations/abstractEncoderImplementation.h"
#include "../../sensorConstants.h"
#include "../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotParts {

class EncoderSensor : public QObject
{
	Q_OBJECT

public:
	EncoderSensor(robotImplementations::sensorImplementations::AbstractEncoderImplementation *encoderImpl, enums::outputPort::OutputPortEnum const &port);
	virtual ~EncoderSensor();
	virtual void read();
	virtual void nullificate();
	robotImplementations::sensorImplementations::AbstractEncoderImplementation *encoderImpl();
	void setImplementation(robotImplementations::sensorImplementations::AbstractEncoderImplementation *encoderImpl);

protected:
	enums::outputPort::OutputPortEnum mPort;
	robotImplementations::sensorImplementations::AbstractEncoderImplementation *mEncoderImpl;  // Doesn't have ownership
};
}
}
}
}
}
