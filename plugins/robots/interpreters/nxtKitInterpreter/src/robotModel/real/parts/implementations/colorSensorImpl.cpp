#include "colorSensorImpl.h"

#include <utils/tracer.h>

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

int const maxColorValue = 1023;

ColorSensorImpl::ColorSensorImpl(PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator
		, enums::lowLevelSensorType::SensorTypeEnum lowLevelType)
	: mImplementation(robotCommunicator, port, lowLevelType, enums::sensorMode::RAWMODE)
	, mLowLevelType(lowLevelType)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &ColorSensorImpl::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &ColorSensorImpl::configurationCompleted);
}

void ColorSensorImpl::read()
{
	if (!mImplementation.isConfigured()) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
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

void ColorSensorImpl::doConfiguration()
{
	mImplementation.configure();
}

void ColorSensorImpl::sensorSpecificProcessResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		utils::Tracer::debug(utils::Tracer::sensors, "BluetoothColorSensorImplementation::sensorSpecificProcessResponse"
				, "Something is wrong, response is empty");
	} else {
		utils::Tracer::debug(utils::Tracer::sensors, "BluetoothColorSensorImplementation::sensorSpecificProcessResponse"
				, "Data received "
				+ QString::number((0xff & reading[8])) + " " + QString::number((0xff & reading[9])) + " "
				+ QString::number((0xff & reading[10])) + " " + QString::number((0xff & reading[11])) + " "
				+ QString::number((0xff & reading[12])) + " " + QString::number((0xff & reading[13])) + " "
				+ QString::number((0xff & reading[14])) + " " + QString::number((0xff & reading[15])) + " "
			);
		mImplementation.setState(NxtInputDevice::idle);
		if (mLowLevelType == enums::lowLevelSensorType::COLORFULL) {
			emit newData(0xff & reading[14]);  // Scaled value, used in ColorFull mode.
		} else {
			emit newData(((0xff & reading[10]) | ((0xff & reading[11]) << 8)) * 100 / maxColorValue);
		}
	}
}
