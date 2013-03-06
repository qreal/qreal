#include <QtCore/QMetaType>
#include <time.h>

#include "bluetoothRobotCommunicationThread.h"
#include "../../../thirdparty/qextserialport/src/qextserialport.h"
#include "../tracer.h"

unsigned const keepAliveResponseSize = 9;
unsigned const getFirmwareVersionResponseSize = 9;

using namespace qReal::interpreters::robots::details;

BluetoothRobotCommunicationThread::BluetoothRobotCommunicationThread()
		: mPort(NULL)
		, mKeepAliveTimer(new QTimer(this))
{
	qRegisterMetaType<inputPort::InputPortEnum>("details::inputPort::InputPortEnum");

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
	if (buffer.size() >= 3 && buffer[2] == errorCode::success) {
		QByteArray const result = receive(responseSize);
		emit response(addressee, result);
	} else {
		emit response(addressee, QByteArray());
	}
}

void BluetoothRobotCommunicationThread::connect(QString const &portName)
{
	if (mPort != NULL) {
		disconnect();
		SleeperThread::msleep(1000);  // Give port some time to close
	}

	mPort = new QextSerialPort(portName, QextSerialPort::Polling);
	mPort->setBaudRate(BAUD9600);
	mPort->setFlowControl(FLOW_OFF);
	mPort->setParity(PAR_NONE);
	mPort->setDataBits(DATA_8);
	mPort->setStopBits(STOP_2);
	mPort->setTimeout(3000);

	mPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	Tracer::debug(tracer::initialization, "BluetoothRobotCommunicationThread::connect"
			, "Port " + mPort->portName() + " is open: " + QString("%1").arg(mPort->isOpen()));

	// Sending "Get firmware version" system command to check connection.
	QByteArray command(4, 0);
	command[0] = 0x02;  //command length
	command[1] = 0x00;
	command[2] = 0x01;
	command[3] = 0x88;

	send(command);
	QByteArray const response = receive(getFirmwareVersionResponseSize);

	emit connected(response != QByteArray());

	mKeepAliveTimer->start(500);
}

void BluetoothRobotCommunicationThread::reconnect(QString const &portName)
{
	connect(portName);
}

void BluetoothRobotCommunicationThread::disconnect()
{
	if (mPort) {
		mPort->close();
		delete mPort;
		mPort = NULL;
		mKeepAliveTimer->stop();
	}
	emit disconnected();
}

void BluetoothRobotCommunicationThread::sendI2C(QObject *addressee
		, QByteArray const &buffer, unsigned const responseSize
		, inputPort::InputPortEnum const &port)
{
	if (!mPort) {
		emit response(addressee, QByteArray());
		return;
	}
	RobotCommunicationThreadBase::sendI2C(addressee, buffer, responseSize, port);
}

void BluetoothRobotCommunicationThread::allowLongJobs(bool allow)
{
	Q_UNUSED(allow)
}

void BluetoothRobotCommunicationThread::send(QByteArray const &buffer
		, unsigned const responseSize, QByteArray &outputBuffer)
{
	send(buffer);
	outputBuffer = receive(responseSize);
}

void BluetoothRobotCommunicationThread::send(QByteArray const &buffer) const
{
	Tracer::debug(tracer::robotCommunication, "BluetoothRobotCommunicationThread::send", "Sending:");
	for (int i = 0; i < buffer.size(); ++i) {
		Tracer::debug(tracer::robotCommunication, "BluetoothRobotCommunicationThread::send"
				, QString("Byte %1 %2").arg(i).arg(static_cast<unsigned char>(buffer[i])));
	}
	mPort->write(buffer);
}

QByteArray BluetoothRobotCommunicationThread::receive(int size) const
{
	QByteArray const result = mPort->read(size);

	Tracer::debug(tracer::robotCommunication, "BluetoothRobotCommunicationThread::receive", "Received:");
	for (int i = 0; i < result.size(); ++i) {
		Tracer::debug(tracer::robotCommunication, "BluetoothRobotCommunicationThread::receive"
				, QString("Byte %1 %2").arg(i).arg(static_cast<unsigned char>(result[i])));
	}

	return result;
}

void BluetoothRobotCommunicationThread::checkForConnection()
{
	if (!mPort || !mPort->isOpen()) {
		return;
	}

	QByteArray command(4, 0);
	command[0] = 0x02;
	command[1] = 0x00;

	command[2] = telegramType::directCommandResponseRequired;
	command[3] = commandCode::KEEPALIVE;

	send(command);

	QByteArray const response = receive(keepAliveResponseSize);

	if (response == QByteArray()) {
		emit disconnected();
	}
}

void BluetoothRobotCommunicationThread::checkConsistency()
{
}
