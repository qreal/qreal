#include "bluetoothRobotCommunicationThread.h"

#include <QtCore/QMetaType>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <time.h>

#include <qrkernel/settingsManager.h>
#include <plugins/robots/thirdparty/qextserialport/src/qextserialport.h>
#include <utils/tracer.h>

#include "commandConstants.h"

unsigned const keepAliveResponseSize = 5;

using namespace ev3KitInterpreter::communication;

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
		, QByteArray const &buffer, unsigned const responseSize)
{
	if (!mPort) {
		emit response(addressee, QByteArray());
		return;
	}

	send(buffer);
	if (buffer.size() >= 5 && buffer[4] == DIRECT_COMMAND_REPLY) {
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

	QString const portName = qReal::SettingsManager::value("Ev3BluetoothPortName").toString();
	mPort = new QextSerialPort(portName, QextSerialPort::Polling);
	mPort->setBaudRate(BAUD9600);
	mPort->setFlowControl(FLOW_OFF);
	mPort->setParity(PAR_NONE);
	mPort->setDataBits(DATA_8);
	mPort->setStopBits(STOP_2);
	mPort->setTimeout(3000);

	mPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	//utils::Tracer::debug(utils::Tracer::initialization, "BluetoothRobotCommunicationThread::connect"
	//		, "Port " + mPort->portName() + " is open: " + QString("%1").arg(mPort->isOpen()));

	// Sending "Keep alive" command to check connection.
	keepAlive();
	QByteArray const response = receive(keepAliveResponseSize);

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
	Q_UNUSED(allow);
}

void BluetoothRobotCommunicationThread::send(QByteArray const &buffer
		, unsigned const responseSize, QByteArray &outputBuffer)
{
	send(buffer);
	outputBuffer = receive(responseSize);
}

void BluetoothRobotCommunicationThread::send(QByteArray const &buffer) const
{
	//utils::Tracer::debug(utils::Tracer::robotCommunication, "BluetoothRobotCommunicationThread::send", "Sending:");
	//for (int i = 0; i < buffer.size(); ++i) {
	//	utils::Tracer::debug(utils::Tracer::robotCommunication, "BluetoothRobotCommunicationThread::send"
	//			, QString("Byte %1 %2").arg(i).arg(static_cast<unsigned char>(buffer[i])));
	//}

	mPort->write(buffer);
}

QByteArray BluetoothRobotCommunicationThread::receive(int size) const
{
	QByteArray const result = mPort->read(size);

	//utils::Tracer::debug(utils::Tracer::robotCommunication, "BluetoothRobotCommunicationThread::receive", "Received:");
	//for (int i = 0; i < result.size(); ++i) {
	//	utils::Tracer::debug(utils::Tracer::robotCommunication, "BluetoothRobotCommunicationThread::receive"
	//			, QString("Byte %1 %2").arg(i).arg(static_cast<unsigned char>(result[i])));
	//}

	return result;
}

void BluetoothRobotCommunicationThread::checkForConnection()
{
	if (!mPort || !mPort->isOpen()) {
		return;
	}

	keepAlive();
	QByteArray const response = receive(keepAliveResponseSize);

	if (response == QByteArray()) {
		emit disconnected();
	}
}

void BluetoothRobotCommunicationThread::keepAlive()
{
	QByteArray command(9, 0);
	command[0] = 7;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = 0x00;
	command[4] = DIRECT_COMMAND_REPLY;
	int globalVariablesCount = 0;
	int localVariablesCount = 0;
	command[5] = globalVariablesCount & 0xFF;
	command[6] = ((localVariablesCount << 2) | (globalVariablesCount >> 8));
	command[7] = opKEEP_ALIVE;
	command[8] = 10;
	send(command);
}

void BluetoothRobotCommunicationThread::checkConsistency()
{
}
