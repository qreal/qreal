#pragma once

#include <QtNetwork/QTcpSocket>

namespace robots {
namespace trikGenerator {

/// Class that handles connection to robot and sends commands to it.
class TcpRobotCommunicator
{
public:
	TcpRobotCommunicator();
	virtual ~TcpRobotCommunicator();

	/// Reads generated program from a file and uploads it to a robot using "file" command.
	bool uploadProgram(QString const &programName);

	/// Sends a command to run previously uploaded file in a robot.
	bool runProgram(QString const &programName);

private:
	/// Establishes connection and initializes socket. If connection fails, leaves socket
	/// in invalid state.
	void connect();

	/// Disconnects from robot.
	void disconnect();

	/// Socket that holds connection.
	QTcpSocket mSocket;
};

}
}
