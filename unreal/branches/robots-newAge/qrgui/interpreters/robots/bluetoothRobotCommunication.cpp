#include "bluetoothRobotCommunication.h"

#include <QtCore/QDebug>

#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialenumerator.h"
#include "../../thirdparty/qextserialport-1.2win-alpha/qextserialport.h"

using namespace qReal::interpreters::robots;

BluetoothRobotCommunication::BluetoothRobotCommunication(QString const &portName)
	: mPortName(portName)
	, mPort(NULL)
{
}

void BluetoothRobotCommunication::send(QByteArray const &buffer)
{
	qDebug() << "Sending: ";
	mPort->write(buffer);
}

void BluetoothRobotCommunication::connect()
{
	qDebug() << "BluetoothRobotCommunication::connect";

//	mPort = new QextSerialPort("COM" + QString::number(comPort));
	mPort = new QextSerialPort(mPortName);
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
}

void BluetoothRobotCommunication::disconnect()
{
	qDebug() << "BluetoothRobotCommunication::disconnect";
	mPort->close();
	delete mPort;
	mPort = NULL;
}

void BluetoothRobotCommunication::setPortName(QString const &portName)
{
	bool needReconnect = (portName != mPortName) && mPort->isOpen();
	mPortName = portName;
	if (needReconnect) {
		disconnect();
		connect();
	}
}
