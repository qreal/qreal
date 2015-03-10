#include "motionSensor.h"

#include <utils/tcpRobotCommunicator.h>
#include <qrutils/inFile.h>

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

MotionSensor::MotionSensor(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikMotionSensor(info, port)
	, mRobotCommunicator(tcpRobotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &MotionSensor::onIncomingData);
}

void MotionSensor::read()
{
	const QString pathToCommand = ":/trikQts/templates/wait/motion.t";
	const QString directCommand = utils::InFile::readAll(pathToCommand) + "script.run()";
	mRobotCommunicator.runDirectCommand(directCommand);

	mRobotCommunicator.requestData(port().name());
}

void MotionSensor::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}
