#include "colorSensorImpl.h"

unsigned const colorSensorResponseSize = 9;

using namespace ev3KitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

ColorSensorImpl::ColorSensorImpl(PortInfo const &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator
		, enums::sensorType::SensorTypeEnum lowLevelType)
	: mImplementation(robotCommunicator, port)
	, mRobotCommunicator(robotCommunicator)
	, mLowLevelType(lowLevelType)
{
}

void ColorSensorImpl::read()
{
	QByteArray command = mImplementation.readyRawCommand(mImplementation.lowLevelPort(), 0);
	QByteArray outputBuf;
	mRobotCommunicator.send(command, colorSensorResponseSize, outputBuf);

	/// @todo Debug this.
	if (mLowLevelType == enums::sensorType::SensorTypeEnum::COLOR_FULL)
		emit newData(1);
		//emit newData(int(outputBuf.data()[5]));
	else if (static_cast<int>(outputBuf.data()[6]) < 20) {
		emit newData(1);
	} else {
		emit newData(1);
	}
}
