#pragma once

#include <QtNetwork/QTcpSocket>
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace trik {

/// Class that handles connection to robot and sends commands to it.
class ROBOTS_GENERATOR_EXPORT TcpRobotCommunicator
{
public:
	TcpRobotCommunicator();
	virtual ~TcpRobotCommunicator();

	/// Reads generated program from a file and uploads it to a robot using "file" command.
	bool uploadProgram(QString const &programName);

	/// Sends a command to run previously uploaded file in a robot.
	bool runProgram(QString const &programName);

	/// Sends a script to be executed directly, without a need for a file.
	bool runDirectCommand(QString const &directCommand);

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
