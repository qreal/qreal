#pragma once

#include "robotCommunicationInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {

class BluetoothRobotCommunication : public RobotCommunicationInterface
{
public:
	BluetoothRobotCommunication();

	virtual void send(QByteArray const &buffer);
	virtual void connect(unsigned int comPort);
	virtual void disconnect();
};

}
}
}
