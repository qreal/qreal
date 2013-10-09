#pragma once

#include <QtNetwork/QTcpSocket>

namespace qReal {
namespace robots {
namespace generators {
namespace trik {

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

	/// Sends a command to remotely abort script execution and stop robot.
	bool stopRobot();

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
}
}
