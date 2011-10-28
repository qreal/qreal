#pragma once

#include "abstractEncoderImplementation.h"
#include "../../robotCommunication/robotCommunication.h"

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
	BluetoothEncoderImplementation(RobotCommunication *robotCommunicationInterface
			,outputPort::OutputPortEnum const &port);
	virtual void read();
	virtual void nullificate();

protected:
	virtual void sensorSpecificProcessResponse(QByteArray const &reading);
	RobotCommunication *mRobotCommunicationInterface;

protected slots:
	void readingDone(QObject *addressee, QByteArray const &reading);

};
}
}
}
}
}
}
