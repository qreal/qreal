#pragma once

#include <QtNetwork/QTcpSocket>

#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "utilsDeclSpec.h"

namespace utils {

/// Class that handles connection to robot and sends commands to it.
class ROBOTS_UTILS_EXPORT TcpRobotCommunicator : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param serverIpSettingsKey - where to find server ip setting in a registry.
	TcpRobotCommunicator(QString const &serverIpSettingsKey);
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
	void connected(bool result, QString const &errorString);
	void disconnected();

private:
	/// Socket that holds connection.
	QTcpSocket mSocket;

	bool mIsConnected;
	QString const mServerIpSettingsKey;
};

}
