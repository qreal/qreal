#include "encoderSensor.h"

#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

EncoderSensor::EncoderSensor(const DeviceInfo &info, const PortInfo &port
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
