#pragma once

#include <QtNetwork/QTcpSocket>

#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace trik {

/// Class that handles connection to robot and sends commands to it.
class TcpRobotCommunicator : public QObject
{
	Q_OBJECT

public:
	explicit TcpRobotCommunicator(qReal::ErrorReporterInterface &errorReporter);
	~TcpRobotCommunicator();

	/// Reads generated program from a file and uploads it to a robot using "file" command.
	bool uploadProgram(QString const &programName);

	/// Sends a command to run previously uploaded file in a robot.
	bool runProgram(QString const &programName);

	/// Sends a script to be executed directly, without a need for a file.
	bool runDirectCommand(QString const &directCommand);

	/// Sends a command to remotely abort script execution and stop robot.
	bool stopRobot();

private slots:
	void onIncomingData();
	void processIncomingMessage(QString const &message);

private:
	/// Establishes connection and initializes socket. If connection fails, leaves socket
	/// in invalid state.
	void connect();

	/// Disconnects from robot.
	void disconnect();

	/// Sends message using message length protocol (message is in form "<data length in bytes>:<data>").
	void send(QString const &data);

	/// Socket that holds connection.
	QTcpSocket mSocket;

	/// Reference to error reporter.
	qReal::ErrorReporterInterface &mErrorReporter;

	/// Buffer to accumulate parts of a message.
	QByteArray mBuffer;

	/// Declared size of a current message.
	int mExpectedBytes = 0;
};

}
