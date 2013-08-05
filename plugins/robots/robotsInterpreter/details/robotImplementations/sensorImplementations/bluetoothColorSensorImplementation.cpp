#include "bluetoothColorSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

int const maxColorValue = 1023;

BluetoothColorSensorImplementation::BluetoothColorSensorImplementation(
		RobotCommunicator *robotCommunicationInterface
		, robots::enums::inputPort::InputPortEnum const port
		, enums::lowLevelSensorType::SensorTypeEnum mode
		, robots::enums::sensorType::SensorTypeEnum const sensorType)
	: BluetoothSensorImplementation(robotCommunicationInterface, sensorType, mode, enums::sensorMode::RAWMODE, port)
{
}

void BluetoothColorSensorImplementation::read()
{
	if (!mIsConfigured) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
		emit failure();
		return;
	}

	if (mState == pending)
		return;
	mState = pending;

	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETINPUTVALUES;
	command[4] = mPort;
	mRobotCommunicationInterface->send(this, command, 18);
}

void BluetoothColorSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		Tracer::debug(tracer::enums::sensors, "BluetoothColorSensorImplementation::sensorSpecificProcessResponse", "Something is wrong, response is empty");
	} else {
		Tracer::debug(tracer::enums::sensors, "BluetoothColorSensorImplementation::sensorSpecificProcessResponse"
				, "Data received "
				+ QString::number((0xff & reading[8])) + " " + QString::number((0xff & reading[9])) + " "
				+ QString::number((0xff & reading[10])) + " " + QString::number((0xff & reading[11])) + " "
				+ QString::number((0xff & reading[12])) + " " + QString::number((0xff & reading[13])) + " "
				+ QString::number((0xff & reading[14])) + " " + QString::number((0xff & reading[15])) + " "
			);
		mState = idle;
		if (mLowLevelSensorType == enums::lowLevelSensorType::COLORFULL) {
			emit response(0xff & reading[14]);  // Scaled value, used in ColorFull mode.
		} else {
			emit response(((0xff & reading[10]) | ((0xff & reading[11]) << 8)) * 100 / maxColorValue);
		}
	}
}

void BluetoothColorSensorImplementation::reconfigure(enums::lowLevelSensorType::SensorTypeEnum mode)
{
	mLowLevelSensorType = mode;
	mIsConfigured = false;
	mResetDone = false;
	configure();
}
