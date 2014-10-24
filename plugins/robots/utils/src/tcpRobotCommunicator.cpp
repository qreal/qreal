#include <utils/tcpRobotCommunicator.h>

#include <QtNetwork/QHostAddress>
#include <QtCore/QFileInfo>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>
#include <qrutils/inFile.h>

using namespace utils;

static uint const port = 8888;
static uint const telemetryPort = 9000;

TcpRobotCommunicator::TcpRobotCommunicator(QString const &settings)
	: mErrorReporter(nullptr)
	, mSettings(settings)
{
	QObject::connect(&mSocket, SIGNAL(readyRead()), this, SLOT(onIncomingControlData()), Qt::DirectConnection);
	QObject::connect(&mTelemetrySocket, SIGNAL(readyRead()), this, SLOT(onIncomingSensorsData()), Qt::DirectConnection);
}

TcpRobotCommunicator::~TcpRobotCommunicator()
{
	disconnect();
}

bool TcpRobotCommunicator::uploadProgram(QString const &programName)
{
	if (programName.isEmpty()) {
		return false;
	}

	QString fileContents;
	try {
		fileContents = utils::InFile::readAll(programName);
	} catch (qReal::Exception const &) {
		return false;
	}

	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	QString const &fileNameOnARobot = QFileInfo(programName).fileName();

	send("file:" + fileNameOnARobot + ":" + fileContents, mSocket);

	return true;
}

bool TcpRobotCommunicator::runProgram(QString const &programName)
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	send("run:" + programName, mSocket);

	return true;
}

bool TcpRobotCommunicator::runDirectCommand(QString const &directCommand)
{
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	send("direct:" + directCommand, mSocket);

	return true;
}

bool TcpRobotCommunicator::stopRobot()
{
	connect();
	if (mSocket.state() != QAbstractSocket::ConnectedState) {
		return false;
	}

	send("stop", mSocket);

	return true;
}

void TcpRobotCommunicator::requestData(QString const &sensor)
{
	if (mTelemetrySocket.state() != QAbstractSocket::ConnectedState) {
		return;
	}

	send("sensor:" + sensor, mTelemetrySocket);
}

void TcpRobotCommunicator::setErrorReporter(qReal::ErrorReporterInterface *errorReporter)
{
	mErrorReporter = errorReporter;
}

void TcpRobotCommunicator::onIncomingControlData()
{
	processIncomingData(mSocket, mBuffer, mExpectedBytes);
}

void TcpRobotCommunicator::onIncomingSensorsData()
{
	processIncomingData(mTelemetrySocket, mTelemetryBuffer, mExpectedBytesFromTelemetry);
}

void TcpRobotCommunicator::processIncomingData(QTcpSocket &socket, QByteArray &buffer, int &expectedBytes)
{
	if (!socket.isValid()) {
		return;
	}

	QByteArray const &data = socket.readAll();
	buffer.append(data);

	while (!buffer.isEmpty()) {
		if (expectedBytes == 0) {
			// Determining the length of a message.
			int const delimiterIndex = buffer.indexOf(':');
			if (delimiterIndex == -1) {
				// We did not receive full message length yet.
				return;
			} else {
				QByteArray const length = buffer.left(delimiterIndex);
				buffer = buffer.mid(delimiterIndex + 1);
				bool ok;
				expectedBytes = length.toInt(&ok);
				if (!ok) {
					QLOG_ERROR() << "Malformed message, can not determine message length from this:" << length;
					expectedBytes = 0;
				}
			}
		} else {
			if (buffer.size() >= expectedBytes) {
				QByteArray const message = buffer.left(expectedBytes);
				buffer = buffer.mid(expectedBytes);

				processIncomingMessage(message);

				expectedBytes = 0;
			} else {
				// We don't have all message yet.
				return;
			}
		}
	}
}

void TcpRobotCommunicator::processIncomingMessage(QString const &message)
{
	QString const errorMarker("error: ");
	QString const infoMarker("info: ");
	QString const sensorMarker("sensor:");

	if (message.startsWith(errorMarker) && mErrorReporter) {
		mErrorReporter->addError(message.mid(errorMarker.length()));
	} else if (message.startsWith(infoMarker) && mErrorReporter) {
		mErrorReporter->addInformation(message.mid(infoMarker.length()));
	} else if (message.startsWith(sensorMarker)){
		QString data(message);
		data.remove(0, sensorMarker.length());
		QStringList portAndValue = data.split(":");
		if (portAndValue[1].startsWith('(')) {
			portAndValue[1].remove(0, 1);
			portAndValue[1].remove(portAndValue[1].length() - 1, 1);
			QStringList stringValues = portAndValue[1].split(",");
			QVector<int> values;
			for (QString const &value : stringValues) {
				values.push_back(value.toInt());
			}

			emit newVectorSensorData(portAndValue[0], values);
		} else {
			emit newScalarSensorData(portAndValue[0], portAndValue[1].toInt());
		}
	} else {
		QLOG_INFO() << "Incoming message of unknown type: " << message;
	}
}

void TcpRobotCommunicator::connect()
{
	QString const server = qReal::SettingsManager::value(mSettings).toString();
	QHostAddress hostAddress(server);
	if (hostAddress.isNull()) {
		QLOG_ERROR() << "Unable to resolve host.";
		return;
	}

	if (mSocket.state() == QTcpSocket::ConnectedState || mSocket.state() == QTcpSocket::ConnectingState) {
		return;
	}

	mSocket.connectToHost(hostAddress, static_cast<quint16>(port));
	bool result = mSocket.waitForConnected(5000);
	if (!result) {
		QLOG_ERROR() << mSocket.errorString();
	}

	mBuffer.clear();
	mExpectedBytes = 0;

	emit connected(result);

	if (mTelemetrySocket.state() == QTcpSocket::ConnectedState || mTelemetrySocket.state() == QTcpSocket::ConnectingState) {
		return;
	}

	mTelemetrySocket.connectToHost(hostAddress, static_cast<quint16>(telemetryPort));
	result = mTelemetrySocket.waitForConnected(5000);
	if (!result) {
		QLOG_ERROR() << mTelemetrySocket.errorString();
	}

	mTelemetryBuffer.clear();
	mExpectedBytesFromTelemetry = 0;

	emit connected(result);
}

void TcpRobotCommunicator::disconnect()
{
	if (mSocket.state() == QTcpSocket::ConnectedState) {
		mSocket.disconnectFromHost();
		mSocket.waitForDisconnected(3000);
	}

	if (mTelemetrySocket.state() == QTcpSocket::ConnectedState) {
		mTelemetrySocket.disconnectFromHost();
		mTelemetrySocket.waitForDisconnected(3000);
	}

	emit disconnected();
}

void TcpRobotCommunicator::send(QString const &data, QTcpSocket &socket)
{
	QByteArray dataByteArray = data.toUtf8();
	dataByteArray = QByteArray::number(dataByteArray.size()) + ':' + dataByteArray;
	socket.write(dataByteArray);
	if (!socket.waitForBytesWritten(3000)) {
		QLOG_ERROR() << "Unable to send data" << data << "to" << socket.peerAddress();
	}
}
