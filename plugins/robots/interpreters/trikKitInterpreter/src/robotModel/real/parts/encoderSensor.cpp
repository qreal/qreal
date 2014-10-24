#include "encoderSensor.h"

#include <utils/tracer.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

EncoderSensor::EncoderSensor(DeviceInfo const &info, PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: interpreterBase::robotModel::robotParts::EncoderSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &EncoderSensor::onIncomingData);
}

void EncoderSensor::read()
{
	mRobotCommunicator.requestData(port().name());
}

void EncoderSensor::onIncomingData(QString const &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

void EncoderSensor::nullify()
{
}
