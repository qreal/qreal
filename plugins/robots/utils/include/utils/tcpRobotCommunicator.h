#pragma once

#include <QtNetwork/QTcpSocket>
#include "utilsDeclSpec.h"

namespace utils {

/// Class that handles connection to robot and sends commands to it.
class ROBOTS_UTILS_EXPORT TcpRobotCommunicator : public QObject
{
	Q_OBJECT

public:
	TcpRobotCommunicator(QString const &settings);
	~TcpRobotCommunicator();

	/// Reads generated program from a file and uploads it to a robot using "file" command.
	bool uploadProgram(QString const &programName);

	/// Sends a command to run previously uploaded file in a robot.
	bool runProgram(QString const &programName);

	/// Sends a script to be executed directly, without a need for a file.
	bool runDirectCommand(QString const &directCommand);

	/// Sends a command to remotely abort script execution and stop robot.
	bool stopRobot();

	/// Establishes connection and initializes socket. If connection fails, leaves socket
	/// in invalid state.
	void connect();

	/// Disconnects from robot.
	void disconnect();

signals:
	/// Return correctness of the connection
	void connected(bool result);
	/// Signal of disconnection
	void disconnected();

private:
	/// Socket that holds connection.
	QTcpSocket mSocket;

	bool mIsConnected;
	QString mSettings;
};

}
