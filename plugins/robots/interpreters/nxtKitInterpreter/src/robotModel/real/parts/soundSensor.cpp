#include "soundSensor.h"
#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

int const soundMaxValue = 1023;

SoundSensor::SoundSensor(DeviceInfo const &info, PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::SoundSensor(info, port)
	, mImplementation(robotCommunicator, port, enums::lowLevelSensorType::SOUND_DBA, enums::sensorMode::RAWMODE)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &SoundSensor::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &SoundSensor::configurationCompleted);
}

void SoundSensor::read()
{
	if (!mImplementation.isConfigured()) {
		emit failure();
		return;
	}

	if (mImplementation.state() == NxtInputDevice::pending) {
		return;
	}

	mImplementation.setState(NxtInputDevice::pending);

	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETINPUTVALUES;
	command[4] = mImplementation.lowLevelPort();
	mImplementation.send(command, 18);
}

void SoundSensor::doConfiguration()
{
	mImplementation.configure();
}

void SoundSensor::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		utils::Tracer::debug(utils::Tracer::sensors, "BluetoothSoundSensorImplementation::sensorSpecificProcessResponse"
				, "Something is wrong, response is empty");
	} else {
		int const sensorValue = (0xff & reading[13]) << 8 | (0xff & reading[14]);
		mImplementation.setState(NxtInputDevice::idle);
		emit newData(sensorValue * 100 / soundMaxValue);
	}
}
