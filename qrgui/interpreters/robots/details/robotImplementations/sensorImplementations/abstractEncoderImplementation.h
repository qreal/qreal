#pragma once
#include <QtCore/QObject>
#include "../../../sensorConstants.h"
#include "../../../robotCommunicationInterface.h"
#include "../../robotCommandConstants.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class AbstractEncoderImplementation : public QObject
{
	Q_OBJECT
public:
	AbstractEncoderImplementation(outputPort::OutputPortEnum const &port);
	virtual ~AbstractEncoderImplementation() {};
	virtual void read() = 0;
	virtual void nullificate() = 0;
	void setPort(outputPort::OutputPortEnum const &port);

signals:
	void response(int reading);
	void failure();

protected:
	enum State {
		idle
		, pending
	};

	virtual void sensorSpecificProcessResponse(QByteArray const &reading);

	outputPort::OutputPortEnum mPort;
	State mState;
};

}
}
}
}
}
}
