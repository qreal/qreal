#pragma once

#include <QtNetwork/QTcpSocket>

#include "utilsDeclSpec.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <utils/tcpConnectionHandler.h>

namespace utils {

/// Class that handles connection to robot and sends commands to it.
class ROBOTS_UTILS_EXPORT TcpRobotCommunicator : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param serverIpSettingsKey - where to find server ip setting in a registry.
	explicit TcpRobotCommunicator(QString const &serverIpSettingsKey);
	~TcpRobotCommunicator();

	/// Reads generated program from a file and uploads it to a robot using "file" command.
	bool uploadProgram(QString const &programName);

	/// Sends a command to run previously uploaded file in a robot.
	bool runProgram(QString const &programName);

	/// Sends a script to be executed directly, without a need for a file.
	/// @param asScript - if true, the state of a robot will be reset before command evalutation.
	bool runDirectCommand(QString const &directCommand, bool asScript = false);

	/// Sends a command to remotely abort script execution and stop robot.
	bool stopRobot();

	void requestData(QString const &sensor);

	/// Establishes connection and initializes socket. If connection fails, leaves socket
	/// in invalid state.
	void connect();

	/// Disconnects from robot.
	void disconnect();

	void setErrorReporter(qReal::ErrorReporterInterface *errorReporter);

signals:
	/// Emitted when tcp socket with robot was opened or failed to open.
	/// @param errorString contains fail reason in that case.
	void connected(bool result, QString const &errorString);

	/// Emitted each time when connection with robot was aborted.
	void disconnected();

	void newScalarSensorData(QString const &port, int data);

	void newVectorSensorData(QString const &port, QVector<int> const &data);

private slots:
	void processControlMessage(QString const &message);
	void processTelemetryMessage(QString const &message);

private:
	/// Sends message using message length protocol (message is in form "<data length in bytes>:<data>").
	void send(QString const &data, QTcpSocket &socket);

	/// Reference to error reporter.
	qReal::ErrorReporterInterface *mErrorReporter;  // Does not have ownership.

	TcpConnectionHandler mControlConnection;
	TcpConnectionHandler mTelemetryConnection;

	bool mIsConnected;
	QString const mServerIpSettingsKey;
};

}
