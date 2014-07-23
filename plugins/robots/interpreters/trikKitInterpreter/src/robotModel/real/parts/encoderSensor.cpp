#include "encoderSensor.h"

#include <utils/tracer.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

EncoderSensor::EncoderSensor(DeviceInfo const &info, PortInfo const &port
	, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: interpreterBase::robotModel::robotParts::EncoderSensor(info, port)
		, mRobotCommunicator(tcpRobotCommunicator)
{
}

void EncoderSensor::read()
{
	emit newData(0);
}

void EncoderSensor::nullify()
{
}
