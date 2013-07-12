#include "bluetoothEncoderImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothEncoderImplementation::BluetoothEncoderImplementation(RobotCommunicator *robotCommunicationInterface
		, enums::outputPort::OutputPortEnum const &port)
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
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETOUTPUTSTATE;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command, 27);
}

void BluetoothEncoderImplementation::nullificate()
{
	QByteArray command(6, 0);
	command[0] = 0x04;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandNoResponse;
	command[3] = enums::commandCode::RESETMOTORPOSITION;
	command[4] = mPort;
	command[5] = false;
	mRobotCommunicationInterface->send(this, command, 0);
}

void BluetoothEncoderImplementation::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee != this) {
		return;
	}
	sensorSpecificProcessResponse(reading);
}

void BluetoothEncoderImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	mState = idle;

	if (reading.isEmpty()) {
		Tracer::debug(tracer::sensors, "BluetoothEncoderImplementation::sensorSpecificProcessResponse", "Something is wrong, response is empty");
	} else {
		unsigned int recieved = (0xff & reading[23]) | ((0xff & reading[24]) << 8)  | ((0xff & reading[25]) << 16) | ((0xff & reading[26]) << 24);

		Tracer::debug(tracer::sensors, "BluetoothEncoderImplementation::sensorSpecificProcessResponse"
				, "Data received "
				+ QString::number((0xff & reading[23])) + " " + QString::number((0xff & reading[24])) + " "
				+ QString::number((0xff & reading[25])) + " " + QString::number((0xff & reading[26])) + " "
			);

		emit response(recieved);
	}
}
