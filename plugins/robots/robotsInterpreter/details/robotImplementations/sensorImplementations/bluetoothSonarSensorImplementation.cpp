#include "bluetoothSonarSensorImplementation.h"

#include "../../tracer.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;

BluetoothSonarSensorImplementation::BluetoothSonarSensorImplementation(RobotCommunication *robotCommunicationInterface
		, inputPort::InputPortEnum const &port)
	: BluetoothSensorImplementation(robotCommunicationInterface, sensorType::sonar, lowLevelSensorType::LOWSPEED_9V, sensorMode::RAWMODE, port)
{
}

void BluetoothSonarSensorImplementation::read()
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
	setMode(sonarMode::SINGLE_SHOT);
}

void BluetoothSonarSensorImplementation::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		Tracer::debug(tracer::sensors, "BluetoothSonarSensorImplementation::sensorSpecificProcessResponse", "Something is wrong, response is empty");
	} else if (reading.size() == 1 && reading[0] == 0) {
		// Sensor configured, now we can send actual request.
		QByteArray command(2, 0);
		command[0] = sonarRegisters::SONAR_ADDRESS;
		command[1] = sonarRegisters::RESULT_1;
		sendCommand(command, 1);
	} else if (reading.size() == 1 && reading[0] != 0) {
		Tracer::debug(tracer::sensors, "BluetoothSonarSensorImplementation::sensorSpecificProcessResponse", "Reading malformed: " + QString::number(static_cast<unsigned int>(reading[0])));
	} else {
		Tracer::debug(tracer::sensors, "BluetoothSonarSensorImplementation::sensorSpecificProcessResponse", "Data received: " + QString::number(0xff & reading[1]) + " cm");
		mState = idle;
		emit response(0xff & reading[1]);
	}
}

void BluetoothSonarSensorImplementation::setMode(sonarMode::SonarModeEnum const &mode)
{
	writeRegister(sonarRegisters::COMMAND, mode);
}

void BluetoothSonarSensorImplementation::writeRegister(sonarRegisters::SonarRegistersEnum const &reg, int value)
{
	QByteArray command(3, 0);
	command[0] = sonarRegisters::SONAR_ADDRESS;
	command[1] = reg;
	command[2] = value;

	sendCommand(command, 0);
}

void BluetoothSonarSensorImplementation::sendCommand(QByteArray const &command, int responseSize)
{
	mRobotCommunicationInterface->sendI2C(this, command, responseSize, static_cast<inputPort::InputPortEnum>(mPort));
}
