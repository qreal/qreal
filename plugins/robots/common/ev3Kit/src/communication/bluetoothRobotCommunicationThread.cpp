/* Copyright 2007-2015 QReal Research Group
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

#include "ev3Kit/communication/bluetoothRobotCommunicationThread.h"

#include <QtCore/QMetaType>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <QtCore/QFileInfo>

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialport.h>

#include "ev3Kit/communication/commandConstants.h"
#include "ev3Kit/communication/ev3DirectCommand.h"

#define     SYSTEM_COMMAND_REPLY              0x01    //  System command, reply required
#define     SYSTEM_COMMAND_NO_REPLY           0x81    //  System command, reply not required
#define     BEGIN_DOWNLOAD                    0x92    //  Begin file down load
#define     CONTINUE_DOWNLOAD                 0x93    //  Continue file down load
#define     SYSTEM_REPLY                      0x03    //  System command reply
#define     SYSTEM_REPLY_ERROR                0x05    //  System command reply error
#define     BEGIN_DOWNLOAD_RESPONSE_SIZE      8
#define     CONTINUE_DOWNLOAD_RESPONSE_SIZE   8
#define     SUCCESS                           0x00
#define     END_OF_FILE                       0x08

const unsigned keepAliveResponseSize = 5;

using namespace ev3::communication;

BluetoothRobotCommunicationThread::BluetoothRobotCommunicationThread()
	: mPort(nullptr)
	, mKeepAliveTimer(new QTimer(this))
{
	QObject::connect(mKeepAliveTimer, SIGNAL(timeout()), this, SLOT(checkForConnection()));
}

BluetoothRobotCommunicationThread::~BluetoothRobotCommunicationThread()
{
	disconnect();
}

void BluetoothRobotCommunicationThread::send(QObject *addressee
		, const QByteArray &buffer, const unsigned responseSize)
{
	if (!mPort) {
		emit response(addressee, QByteArray());
		return;
	}

	send(buffer);
	if (buffer.size() >= 5 && buffer[4] == enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY) {
		QByteArray const result = receive(responseSize);
		emit response(addressee, result);
	} else {
		emit response(addressee, QByteArray());
	}
}

void BluetoothRobotCommunicationThread::connect()
{
	if (mPort) {
		disconnect();
		QThread::msleep(1000);  // Give port some time to close
	}

	const QString portName = qReal::SettingsManager::value("Ev3BluetoothPortName").toString();
	mPort = new QextSerialPort(portName, QextSerialPort::Polling);
	mPort->setBaudRate(BAUD9600);
	mPort->setFlowControl(FLOW_OFF);
	mPort->setParity(PAR_NONE);
	mPort->setDataBits(DATA_8);
	mPort->setStopBits(STOP_2);
	mPort->setTimeout(3000);

	mPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	// Sending "Keep alive" command to check connection.
	keepAlive();
	const QByteArray response = receive(keepAliveResponseSize);

	emit connected(response != QByteArray(), QString());
	mKeepAliveTimer->moveToThread(this->thread());
	mKeepAliveTimer->disconnect();
	QObject::connect(mKeepAliveTimer, SIGNAL(timeout()), this, SLOT(checkForConnection()));
	mKeepAliveTimer->start(500);
}

void BluetoothRobotCommunicationThread::reconnect()
{
	connect();
}

void BluetoothRobotCommunicationThread::disconnect()
{
	if (mPort) {
		mPort->close();
		delete mPort;
		mPort = nullptr;
		mKeepAliveTimer->stop();
	}

	emit disconnected();
}

void BluetoothRobotCommunicationThread::allowLongJobs(bool allow)
{
	Q_UNUSED(allow);
}

void BluetoothRobotCommunicationThread::send(const QByteArray &buffer
		, const unsigned responseSize, QByteArray &outputBuffer)
{
	send(buffer);
	outputBuffer = receive(responseSize);
}

void BluetoothRobotCommunicationThread::send(const QByteArray &buffer) const
{
	mPort->write(buffer);
}

QByteArray BluetoothRobotCommunicationThread::receive(int size) const
{
	return mPort->read(size);
}

void BluetoothRobotCommunicationThread::checkForConnection()
{
	if (!mPort || !mPort->isOpen()) {
		return;
	}

	keepAlive();
	const QByteArray response = receive(keepAliveResponseSize);

	if (response == QByteArray()) {
		emit disconnected();
		mKeepAliveTimer->stop();
	}
}

void BluetoothRobotCommunicationThread::keepAlive()
{
	QByteArray command = Ev3DirectCommand::formCommand(10, 0, 0, 0
			, enums::commandType::CommandTypeEnum::DIRECT_COMMAND_REPLY);
	int index = 7;
	Ev3DirectCommand::addOpcode(enums::opcode::OpcodeEnum::KEEP_ALIVE, command, index);
	Ev3DirectCommand::addByteParameter(10, command, index); // 10 - Number of minutes before entering sleep mode.
	send(command);
}

void BluetoothRobotCommunicationThread::checkConsistency()
{
}

bool BluetoothRobotCommunicationThread::uploadFile(const QString &sourceFile, const QString &targetDir)
{
	const QFileInfo fileInfo(sourceFile);
	// A path to file on the remote device.
	const QString devicePath = targetDir + "/" + fileInfo.fileName();
	QFile file(sourceFile);
	/// @todo: Implement more detailed error reporting
	if (!file.open(QIODevice::ReadOnly)) {
		return false;
	}

	QByteArray data = file.readAll();
	file.close();
	const int chunkSize = 960;

	const int cmdBeginSize = 11 + devicePath.size();
	QByteArray commandBegin(cmdBeginSize, 0);
	commandBegin[0] = (cmdBeginSize - 2) & 0xFF;
	commandBegin[1] = ((cmdBeginSize - 2) >> 8) & 0xFF ;
	commandBegin[2] = 0x02;
	commandBegin[3] = 0x00;
	commandBegin[4] = SYSTEM_COMMAND_REPLY;
	commandBegin[5] = BEGIN_DOWNLOAD;
	commandBegin[6] = data.size() & 0xFF;
	commandBegin[7] = (data.size() >> 8) & 0xFF;
	commandBegin[8] = (data.size() >> 16) & 0xFF;
	commandBegin[9] = (data.size() >> 24) & 0xFF;
	int index = 10;
	for (int i = 0; i < devicePath.size(); i++) {
		commandBegin[index++] = devicePath.at(i).toLatin1();
	}

	commandBegin[index] = 0x00;

	send(commandBegin);
	QByteArray commandBeginResponse = receive(BEGIN_DOWNLOAD_RESPONSE_SIZE);
	if (commandBeginResponse.at(4) != SYSTEM_REPLY) {
		return false;
	}

	char handle = commandBeginResponse.at(7);
	int sizeSent = 0;
	while(sizeSent < data.size()) {
		const int sizeToSend = qMin(chunkSize, data.size() - sizeSent);
		const int cmdContinueSize = 7 + sizeToSend;
		QByteArray commandContinue(cmdContinueSize, 0);
		commandContinue[0] = (cmdContinueSize - 2) & 0xFF;
		commandContinue[1] = ((cmdContinueSize - 2) >> 8) & 0xFF ;
		commandContinue[2] = 0x03;
		commandContinue[3] = 0x00;
		commandContinue[4] = SYSTEM_COMMAND_REPLY;
		commandContinue[5] = CONTINUE_DOWNLOAD;
		commandContinue[6] = handle;
		for (int i = 0; i < sizeToSend; i++) {
			commandContinue[7 + i] = data.at(sizeSent++);
		}

		send(commandContinue);
		QByteArray commandContinueResponse = receive(CONTINUE_DOWNLOAD_RESPONSE_SIZE);
		if (commandContinueResponse.at(7) != SUCCESS &&
				(commandContinueResponse.at(7) != END_OF_FILE && sizeSent == data.size())) {
			return false;
		}
	}

	return true;
}
