#include "accelerometer.h"

using namespace trik::robotModel::real::parts;
using namespace kitBase::robotModel;

Accelerometer::Accelerometer(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &robotCommunicator)
	: kitBase::robotModel::robotParts::AccelerometerSensor(info, port)
	, mRobotCommunicator(robotCommunicator)
{
	connect(&mRobotCommunicator, &utils::TcpRobotCommunicator::newScalarSensorData
			, this, &Accelerometer::onIncomingData);
}

void Accelerometer::read()
{
	mRobotCommunicator.requestData(port().name());
}

void Accelerometer::onIncomingData(const QString &portName, int value)
{
	if (portName == port().name()) {
		emit newData(value);
	}
}
