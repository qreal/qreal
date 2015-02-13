#include "accelerometer.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Accelerometer::Accelerometer(const DeviceInfo &info, const PortInfo &port
		, utils::TcpRobotCommunicator &robotCommunicator)
	: interpreterBase::robotModel::robotParts::AccelerometerSensor(info, port)
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
