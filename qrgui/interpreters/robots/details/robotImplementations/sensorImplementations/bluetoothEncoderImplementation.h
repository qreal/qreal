#pragma once

#include "abstractEncoderImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace sensorImplementations {

class BluetoothEncoderImplementation : public AbstractEncoderImplementation
{
	Q_OBJECT
public:
	BluetoothEncoderImplementation(RobotCommunicationInterface *robotCommunicationInterface
			,outputPort::OutputPortEnum const &port);
	virtual void read();
	virtual void nullificate();

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
	RobotCommunicationInterface *mRobotCommunicationInterface;

protected slots:
	void readingDone(QObject *addressee, QByteArray const &reading);

};
}
}
}
}
}
}
