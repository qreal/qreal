#include "bluetoothRobotCommunicationThread.h"

#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialport.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots::details;

BluetoothRobotCommunicationThread::BluetoothRobotCommunicationThread()
	: mPort(NULL)
{
}

BluetoothRobotCommunicationThread::~BluetoothRobotCommunicationThread()
{
	disconnect();
}

void BluetoothRobotCommunicationThread::send(QObject *addressee, QByteArray const &buffer, unsigned const responseSize)
{
	qDebug() << "Sending...";
	mPort->write(buffer);
	qDebug() << "Receiving...";
	QByteArray result = mPort->read(responseSize);
	qDebug() << "Done.";
	emit response(addressee, result);
}

void BluetoothRobotCommunicationThread::connect(QString portName)
{
	mPort = new QextSerialPort(portName);
	mPort->setBaudRate(BAUD9600);
	mPort->setFlowControl(FLOW_OFF);
	mPort->setParity(PAR_NONE);
	mPort->setDataBits(DATA_8);
	mPort->setStopBits(STOP_2);
	mPort->setTimeout(3000);

	mPort->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

	qDebug() << "Port" << mPort->portName() << "is open:" << mPort->isOpen();

	QByteArray command(4, 0);
	command[0] = 0x02;  //command length
	command[1] = 0x00;
	command[2] = 0x01;
	command[3] = 0x88;

	int bytesTransmitted = mPort->write(command);
	qDebug() << "Trasmitted:" << bytesTransmitted;

	qDebug() << "Reading response";

	QByteArray response = mPort->read(9);
	qDebug() << "Received:" << response;
	emit connected();
}

void BluetoothRobotCommunicationThread::reconnect(QString portName)
{
	disconnect();
	connect(portName);
}

void BluetoothRobotCommunicationThread::disconnect()
{
	mPort->close();
	delete mPort;
	mPort = NULL;
	emit disconnected();
}
