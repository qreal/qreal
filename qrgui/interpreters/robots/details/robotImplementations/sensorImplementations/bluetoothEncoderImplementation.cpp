#include "bluetoothEncoderImplementation.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

BluetoothEncoderImplementation::BluetoothEncoderImplementation(RobotCommunicationInterface *robotCommunicationInterface
		, outputPort::OutputPortEnum const &port)
	: AbstractEncoderImplementation(port)
{
	mRobotCommunicationInterface = robotCommunicationInterface;
	connect(mRobotCommunicationInterface, SIGNAL(response(QObject*,QByteArray))
			, this, SLOT(readingDone(QObject*,QByteArray)));
}

void BluetoothEncoderImplementation::read()
{
	if (mState == pending)
		return;

	mState = pending;
	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandResponseRequired;
	command[3] = commandCode::GETOUTPUTSTATE;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command, 27);
}

void BluetoothEncoderImplementation::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee != this)
		return;
	sensorSpecificProcessResponse(reading);
}

void BluetoothEncoderImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		qDebug() << "Something is wrong, response is empty";
	} else {
		unsigned int recieved = (0xff & reading[15]) | ((0xff & reading[16]) << 8)  | ((0xff & reading[17]) << 16) | ((0xff & reading[18]) << 24);
		qDebug() << "recieved in bluetoothEncoderImplementation.cpp" << recieved;
		emit response(recieved);
	}
}

