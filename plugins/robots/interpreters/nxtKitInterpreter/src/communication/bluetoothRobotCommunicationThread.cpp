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

#include "bluetoothRobotCommunicationThread.h"

#include <QtCore/QMetaType>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <time.h>

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialport.h>

#include "commandConstants.h"

const unsigned keepAliveResponseSize = 9;
const unsigned getFirmwareVersionResponseSize = 9;

using namespace nxt::communication;

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
	if (buffer.size() >= 3 && buffer[2] == enums::errorCode::success) {
		const QByteArray result = receive(responseSize);
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

	const QString portName = qReal::SettingsManager::value("NxtBluetoothPortName").toString();
	mPort = new QextSerialPort(portName, QextSerialPort::Polling);
	mPort->setBaudRate(BAUD9600);
	mPort->setFlowControl(FLOW_OFF);
	mPort->setParity(PAR_NONE);
	mPort->setDataBits(DATA_8);
	mPort->setStopBits(STOP_2);
	mPort->setTimeout(3000);

	mPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	// Sending "Get firmware version" system command to check connection.
	QByteArray command(4, 0);
	command[0] = 0x02;  //command length
	command[1] = 0x00;
	command[2] = 0x01;
	command[3] = 0x88;

	send(command);
	const QByteArray response = receive(getFirmwareVersionResponseSize);

	emit connected(response != QByteArray(), QString());

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
	Q_UNUSED(allow)
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

	QByteArray command(4, 0);
	command[0] = 0x02;
	command[1] = 0x00;

	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::KEEPALIVE;

	send(command);

	const QByteArray response = receive(keepAliveResponseSize);

	if (response == QByteArray()) {
		emit disconnected();
	}
}

void BluetoothRobotCommunicationThread::checkConsistency()
{
}
