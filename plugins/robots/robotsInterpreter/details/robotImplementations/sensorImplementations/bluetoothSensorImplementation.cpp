#include "bluetoothSensorImplementation.h"
#include "../../robotParts/robotModel.h"
#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothSensorImplementation::BluetoothSensorImplementation(RobotCommunicator *const robotCommunicationInterface
		, sensorType::SensorTypeEnum const &sensorType
		, lowLevelSensorType::SensorTypeEnum const &lowLevelSensorType
		, sensorMode::SensorModeEnum const &sensorMode
		, inputPort::InputPortEnum const &port)
	: AbstractSensorImplementation(port, sensorType)
	, mRobotCommunicationInterface(robotCommunicationInterface)
	, mLowLevelSensorType(lowLevelSensorType)
	, mSensorMode(sensorMode)
	, mIsConfigured(false)
	, mResetDone(false)
{
	connect(mRobotCommunicationInterface, SIGNAL(response(QObject*,QByteArray))
			, this, SLOT(readingDone(QObject*,QByteArray)));
}

BluetoothSensorImplementation::~BluetoothSensorImplementation()
{
}

void BluetoothSensorImplementation::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee != this)
		return;
	processResponse(reading);
}

void BluetoothSensorImplementation::processResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		mState = idle;
		if (mResetDone) {
			Tracer::debug(tracer::sensors, "BluetoothSensorImplementation::processResponse", "Response is empty, seems to be a connection failure");
			// Just ignore connection failures for now
//			emit failure();
		}
	} else if (reading.size() >= 5 && reading[3] == commandCode::RESETINPUTSCALEDVALUE) {
		Tracer::debug(tracer::sensors, "BluetoothSensorImplementation::processResponse", "Response is a reset input scaled value response package");
		Tracer::debug(tracer::sensors, "BluetoothSensorImplementation::processResponse", "Status byte is: " + QString::number(static_cast<int>(reading[4])));
		mState = idle;
		mResetDone = true;
		emit configured();
	} else if (reading.size() >= 5 && reading[3] == commandCode::SETINPUTMODE) {
		mState = idle;
		Tracer::debug(tracer::sensors, "BluetoothSensorImplementation::processResponse", "Response is a configuration response package");
		Tracer::debug(tracer::sensors, "BluetoothSensorImplementation::processResponse", "Status byte is: " + QString::number(static_cast<int>(reading[4])));
		QByteArray command(5, 0);
		command[0] = 0x03;
		command[1] = 0x00;
		command[2] = telegramType::directCommandResponseRequired;
		command[3] = commandCode::RESETINPUTSCALEDVALUE;
		command[4] = mPort;
		mRobotCommunicationInterface->send(this, command, 5);

		mIsConfigured = true;
	} else {
		sensorSpecificProcessResponse(reading);
	}
}

void BluetoothSensorImplementation::configure()
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandResponseRequired;
	command[3] = commandCode::SETINPUTMODE;
	command[4] = mPort;
	command[5] = mLowLevelSensorType;
	command[6] = mSensorMode;
	mRobotCommunicationInterface->send(this, command, 5);
}
