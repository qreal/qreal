#include "encoderSensor.h"

#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

EncoderSensor::EncoderSensor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: kitBase::robotModel::robotParts::EncoderSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &EncoderSensor::onIncomingData);
}

void EncoderSensor::read()
{
	mRobotCommunicator.requestData(port().name());
}

void EncoderSensor::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}

void EncoderSensor::nullify()
{
	const QString pathToCommand = ":/trikQts/templates/engines/nullifyEncoder.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand).replace("@@PORT@@", port().name());
	mRobotCommunicator.runDirectCommand(directCommand);
}
