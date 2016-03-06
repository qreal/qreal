/* Copyright 2007-2016 QReal Research Group, Dmitry Mordvinov, Yurii Litvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "utils/robotCommunication/tcpRobotCommunicator.h"

#include <QtCore/QFileInfo>

#include <qrkernel/logging.h>
#include <qrutils/inFile.h>

#include "src/robotCommunication/tcpRobotCommunicatorWorker.h"

using namespace utils::robotCommunication;

TcpRobotCommunicator::TcpRobotCommunicator(const QString &serverIpSettingsKey)
{
	mWorker.reset(new TcpRobotCommunicatorWorker(serverIpSettingsKey));

	mWorker->moveToThread(&mWorkerThread);

	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::connected
			, this, &TcpRobotCommunicator::onConnected, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::disconnected
			, this, &TcpRobotCommunicator::disconnected, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::newScalarSensorData
			, this, &TcpRobotCommunicator::newScalarSensorData, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::newVectorSensorData
			, this, &TcpRobotCommunicator::newVectorSensorData, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::printText
			, this, &TcpRobotCommunicator::printText, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::startedRunning
			, this, &TcpRobotCommunicator::startedRunning, Qt::QueuedConnection);

	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::messageFromRobot
			, this, &TcpRobotCommunicator::onMessageFromRobot, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::connectionError
			, this, &TcpRobotCommunicator::onConnectionError, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::trikRuntimeVersionError
			, this, &TcpRobotCommunicator::trikRuntimeVersionError, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::trikRuntimeVersionGettingError
			, this, &TcpRobotCommunicator::trikRuntimeVersionGettingError, Qt::QueuedConnection);

	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::uploadProgramDone
			, this, &TcpRobotCommunicator::uploadProgramDone, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::stopRobotDone
			, this, &TcpRobotCommunicator::stopRobotDone, Qt::QueuedConnection);
	QObject::connect(mWorker.data(), &TcpRobotCommunicatorWorker::runDirectCommandDone
			, this, &TcpRobotCommunicator::runDirectCommandDone, Qt::QueuedConnection);

	mWorkerThread.start();

	QMetaObject::invokeMethod(mWorker.data(), "init");
}

TcpRobotCommunicator::~TcpRobotCommunicator()
{
	QMetaObject::invokeMethod(mWorker.data(), "deinit");

	if (mWorkerThread.isRunning()) {
		mWorkerThread.quit();
		mWorkerThread.wait();
	}
}

void TcpRobotCommunicator::uploadProgram(const QString &programName)
{
	if (programName.isEmpty()) {
		QLOG_ERROR() << "Empty program name";
		emit uploadProgramError(tr("Empty program name, can not upload"));
		return;
	}

	QString errorString;
	const QString fileContents = utils::InFile::readAll(programName, &errorString);
	if (!errorString.isEmpty()) {
		QLOG_ERROR() << "Reading file to transfer failed";
		emit uploadProgramError(tr("Can not read generated file, uploading aborted"));
		return;
	}

	const QString fileNameOnARobot = QFileInfo(programName).fileName();

	QMetaObject::invokeMethod(mWorker.data(), "uploadProgram"
			, Q_ARG(QString, fileNameOnARobot), Q_ARG(QString, fileContents));
}

void TcpRobotCommunicator::runProgram(const QString &programName)
{
	QMetaObject::invokeMethod(mWorker.data(), "runProgram", Q_ARG(QString, programName));
}

void TcpRobotCommunicator::runDirectCommand(const QString &directCommand, bool asScript)
{
	QMetaObject::invokeMethod(mWorker.data(), "runDirectCommand"
			, Q_ARG(QString, directCommand), Q_ARG(bool, asScript));
}

void TcpRobotCommunicator::stopRobot()
{
	QMetaObject::invokeMethod(mWorker.data(), "stopRobot");
}

void TcpRobotCommunicator::requestData(const QString &sensor)
{
	QMetaObject::invokeMethod(mWorker.data(), "requestData", Q_ARG(QString, sensor));
}

void TcpRobotCommunicator::connect()
{
	QMetaObject::invokeMethod(mWorker.data(), "connect");
}

void TcpRobotCommunicator::disconnect()
{
	QMetaObject::invokeMethod(mWorker.data(), "disconnect");
}

void TcpRobotCommunicator::onMessageFromRobot(const MessageKind &messageKind, const QString &message)
{

	switch (messageKind) {
	case MessageKind::error:
		emit errorFromRobot(message);
		break;
	case MessageKind::info:
		emit infoFromRobot(message);
		break;
	case MessageKind::text:
		emit printText(message);
		break;
	}
}

void TcpRobotCommunicator::onConnectionError(const QString &error)
{
	emit connectionError(error);

	// Insane protocol for robot model connection require that "connection failed" event is emitted as "connected"
	// signal with "success" flag set to false and error string. Altering this protocol will lead to rewriting half
	// of the interpreter, so we will emit "connectionError" for new robot communication protocol classes and
	// "connected" for models.
	emit connected(false, error);
}

void TcpRobotCommunicator::onConnected()
{
	emit connected(true, "");
}
