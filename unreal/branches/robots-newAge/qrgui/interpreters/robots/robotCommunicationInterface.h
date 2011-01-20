#pragma once

#include <QtCore/QByteArray>

namespace qReal {
namespace interpreters {
namespace robots {

class RobotCommunicationInterface
{
public:
	virtual void send(QByteArray const &buffer) = 0;
	virtual void connect() = 0;
	virtual void disconnect() = 0;
};

}
}
}
