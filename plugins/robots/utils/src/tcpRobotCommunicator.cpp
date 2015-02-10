#include "utils/tcpRobotCommunicator.h"
#include <utils/requiredVersion.h>

#include <QtNetwork/QHostAddress>
#include <QtCore/QFileInfo>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>
#include <qrutils/inFile.h>

using namespace utils;

static const uint controlPort = 8888;
static const uint telemetryPort = 9000;

TcpRobotCommunicator::TcpRobotCommunicator(const QString &serverIpSettingsKey)
	: mErrorReporter(nullptr)
	, mControlConnection(controlPort)
	, mTelemetryConnection(telemetryPort)
	, mIsConnected(false)
	, mServerIpSettingsKey(serverIpSettingsKey)
{
	QObject::connect(&mControlConnection, &TcpConnectionHandler::messageReceived
			, this, &TcpRobotCommunicator::processControlMessage);
	QObject::connect(&mTelemetryConnection, &TcpConnectionHandler::messageReceived
			, this, &TcpRobotCommunicator::processTelemetryMessage);
	QObject::connect(&mVersionTimer, &QTimer::timeout, this, &TcpRobotCommunicator::versionTimeOut);
}

TcpRobotCommunicator::~TcpRobotCommunicator()
{
	disconnect();
}

bool TcpRobotCommunicator::uploadProgram(const QString &programName)
{
	if (programName.isEmpty()) {
		return false;
	}

	QString fileContents;
	try {
		fileContents = utils::InFile::readAll(programName);
	} catch (const qReal::Exception &) {
		return false;
	}

	connect();
	if (!mControlConnection.isConnected()) {
		return false;
	}

	const QString &fileNameOnARobot = QFileInfo(programName).fileName();

	mControlConnection.send("file:" + fileNameOnARobot + ":" + fileContents);

	return true;
}

bool TcpRobotCommunicator::runProgram(const QString &programName)
{
	connect();
	if (!mControlConnection.isConnected()) {
		return false;
	}

	mControlConnection.send("run:" + programName);

	return true;
}

bool TcpRobotCommunicator::runDirectCommand(const QString &directCommand, bool asScript)
{
	if (!mControlConnection.isConnected()) {
		return false;
	}

	const QString command = asScript ? "directScript" : "direct";
	mControlConnection.send(command + ":" + directCommand);

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

void TcpRobotCommunicator::requestData(const QString &sensor)
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

void TcpRobotCommunicator::processControlMessage(const QString &message)
{
	const QString errorMarker("error: ");
	const QString infoMarker("info: ");
	const QString versionMarker("version: ");

	if (message.startsWith(versionMarker) && mErrorReporter) {
		mVersionTimer.stop();
		const QString currentVersion = message.mid(versionMarker.length());
		if (currentVersion != requiredVersion) {
			mErrorReporter->addError(tr("Current TRIK runtime version is not equal to version required by TRIKStudio"));
		}
	} else if (message.startsWith(errorMarker) && mErrorReporter) {
		mErrorReporter->addError(message.mid(errorMarker.length()));
	} else if (message.startsWith(infoMarker) && mErrorReporter) {
		mErrorReporter->addInformation(message.mid(infoMarker.length()));
	} else {
		QLOG_INFO() << "Incoming message of unknown type: " << message;
	}
}

void TcpRobotCommunicator::processTelemetryMessage(const QString &message)
{
	const QString sensorMarker("sensor:");

	if (message.startsWith(sensorMarker)) {
		QString data(message);
		data.remove(0, sensorMarker.length());
		QStringList portAndValue = data.split(":");
		if (portAndValue[1].startsWith('(')) {
			portAndValue[1].remove(0, 1);
			portAndValue[1].remove(portAndValue[1].length() - 1, 1);
			QStringList stringValues = portAndValue[1].split(",");
			QVector<int> values;
			for (const QString &value : stringValues) {
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

void TcpRobotCommunicator::versionTimeOut()
{
	mVersionTimer.stop();
	mErrorReporter->addError(tr("Current TRIK runtime version can not be received"));
}

void TcpRobotCommunicator::versionRequest()
{
	mControlConnection.send("version");
	mVersionTimer.start(3000);
}

void TcpRobotCommunicator::connect()
{
	const QString server = qReal::SettingsManager::value(mServerIpSettingsKey).toString();
	QHostAddress hostAddress(server);
	if (hostAddress.isNull()) {
		QLOG_ERROR() << "Unable to resolve host.";
		return;
	}

	if (mControlConnection.isConnected() && mTelemetryConnection.isConnected()) {
		if (mCurrentIP == server) {
			return;
		}

		disconnect();
	}

	mCurrentIP = server;
	const bool result = mControlConnection.connect(hostAddress) && mTelemetryConnection.connect(hostAddress);
	versionRequest();
	emit connected(result, QString());
}

void TcpRobotCommunicator::disconnect()
{
	mControlConnection.disconnect();
	mTelemetryConnection.disconnect();

	emit disconnected();
}
