#include "encoderSensor.h"

#include <qrutils/inFile.h>

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
	QString const pathToCommand = ":/trikQts/templates/.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand).replace("@@PORT@@", port().name());
	mRobotCommunicator.runDirectCommand(directCommand);
}
