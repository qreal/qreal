#pragma once

#include <QtNetwork/QTcpSocket>
#include "utilsDeclSpec.h"

#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace utils {

/// Class that handles connection to robot and sends commands to it.
class ROBOTS_UTILS_EXPORT TcpRobotCommunicator : public QObject
{
	Q_OBJECT

public:
	explicit TcpRobotCommunicator(QString const &settings);

	~TcpRobotCommunicator();

	/// Reads generated program from a file and uploads it to a robot using "file" command.
	bool uploadProgram(QString const &programName);

	/// Sends a command to run previously uploaded file in a robot.
	bool runProgram(QString const &programName);

	/// Sends a script to be executed directly, without a need for a file.
	bool runDirectCommand(QString const &directCommand);

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
	/// Return correctness of the connection
	void connected(bool result);

	/// Signal of disconnection
	void disconnected();

	void newScalarSensorData(QString const &port, int data);

	void newVectorSensorData(QString const &port, QVector<int> const &data);

private slots:
	void processIncomingData(QTcpSocket &socket, QByteArray &buffer, int &expectedBytes);

	void onIncomingControlData();
	void onIncomingSensorsData();

	void processIncomingMessage(QString const &message);

private:
	/// Sends message using message length protocol (message is in form "<data length in bytes>:<data>").
	void send(QString const &data, QTcpSocket &socket);

	/// Socket that holds connection.
	QTcpSocket mSocket;

	QTcpSocket mTelemetrySocket;

	/// Reference to error reporter.
	qReal::ErrorReporterInterface *mErrorReporter;

	/// Buffer to accumulate parts of a message.
	QByteArray mBuffer;

	QByteArray mTelemetryBuffer;

	/// Declared size of a current message.
	int mExpectedBytes = 0;

	int mExpectedBytesFromTelemetry = 0;

	QString mSettings;
};

}
