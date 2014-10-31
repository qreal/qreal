#include <utils/tcpRobotCommunicator.h>

#include <QtNetwork/QHostAddress>
#include <QtCore/QFileInfo>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>
#include <qrutils/inFile.h>

using namespace utils;

static uint const controlPort = 8888;
static uint const telemetryPort = 9000;

TcpRobotCommunicator::TcpRobotCommunicator(QString const &settings)
	: mErrorReporter(nullptr)
	, mControlConnection(controlPort)
	, mTelemetryConnection(telemetryPort)
	, mSettings(settings)
{
	QObject::connect(&mControlConnection, SIGNAL(messageReceived(QString))
			, this, SLOT(processControlMessage(QString)));
	QObject::connect(&mTelemetryConnection, SIGNAL(messageReceived(QString))
			, this, SLOT(processTelemetryMessage(QString)));
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
	if (!mControlConnection.isConnected()) {
		return false;
	}

	QString const &fileNameOnARobot = QFileInfo(programName).fileName();

	mControlConnection.send("file:" + fileNameOnARobot + ":" + fileContents);

	return true;
}

bool TcpRobotCommunicator::runProgram(QString const &programName)
{
	connect();
	if (!mControlConnection.isConnected()) {
		return false;
	}

	mControlConnection.send("run:" + programName);

	return true;
}

bool TcpRobotCommunicator::runDirectCommand(QString const &directCommand)
{
	if (!mControlConnection.isConnected()) {
		return false;
	}

	mControlConnection.send("direct:" + directCommand);

	return true;
}

bool TcpRobotCommunicator::stopRobot()
{
	if (!mControlConnection.isConnected()) {
		return false;
	}

	mControlConnection.send("stop");

	return true;
}

void TcpRobotCommunicator::requestData(QString const &sensor)
{
	if (!mTelemetryConnection.isConnected()) {
		return;
	}

	mTelemetryConnection.send("sensor:" + sensor);
}

void TcpRobotCommunicator::setErrorReporter(qReal::ErrorReporterInterface *errorReporter)
{
	mErrorReporter = errorReporter;
}

void TcpRobotCommunicator::processControlMessage(QString const &message)
{
	QString const errorMarker("error: ");
	QString const infoMarker("info: ");

	if (message.startsWith(errorMarker) && mErrorReporter) {
		mErrorReporter->addError(message.mid(errorMarker.length()));
	} else if (message.startsWith(infoMarker) && mErrorReporter) {
		mErrorReporter->addInformation(message.mid(infoMarker.length()));
	} else {
		QLOG_INFO() << "Incoming message of unknown type: " << message;
	}
}

void TcpRobotCommunicator::processTelemetryMessage(QString const &message)
{
	QString const sensorMarker("sensor:");

	if (message.startsWith(sensorMarker)) {
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

	if (mControlConnection.isConnected() && mTelemetryConnection.isConnected()) {
		return;
	}

	bool const result = mControlConnection.connect(hostAddress) && mTelemetryConnection.connect(hostAddress);
	emit connected(result);
}

void TcpRobotCommunicator::disconnect()
{
	mControlConnection.disconnect();
	mTelemetryConnection.disconnect();

	emit disconnected();
}
