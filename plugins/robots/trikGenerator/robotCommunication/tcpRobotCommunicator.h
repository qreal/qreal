#pragma once

#include <QtNetwork/QTcpSocket>

namespace robots {
namespace trikGenerator {

class TcpRobotCommunicator
{
public:
	TcpRobotCommunicator();
	bool uploadProgram(QString const &programName);
	bool runProgram(QString const &programName);

private:
	void connect();
	void disconnect();

	QTcpSocket mSocket;
};

}
}
