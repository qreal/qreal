#include "waitForSensorBlock.h"

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

WaitForSensorBlock::WaitForSensorBlock(RobotModelInterface &robotModel
//		, robots::enums::sensorType::SensorTypeEnum type
		)
	: WaitBlock(robotModel)
//	, mType(type)
{
}

WaitForSensorBlock::~WaitForSensorBlock()
{
}

void WaitForSensorBlock::run()
{
	QString const port = stringProperty("Port");

	/// @todo Works only with scalar sensors.
	mPort = RobotModelUtils::findPort(mRobotModel, port, input);
	robotParts::ScalarSensor * const sensor = RobotModelUtils::findDevice<robotParts::ScalarSensor>(mRobotModel, mPort);
	if (sensor) {
		connect(sensor, &robotParts::ScalarSensor::newData, this, &WaitForSensorBlock::responseSlot);
		connect(sensor, &robotParts::AbstractSensor::failure, this, &WaitForSensorBlock::failureSlot);
		sensor->read();
		mActiveWaitingTimer.start();
	} else {
		mActiveWaitingTimer.stop();
		error(tr("%1 is not configured on port %2").arg(name(), mPort.name()));
	}
}

//QList<Block::SensorPortPair> WaitForSensorBlock::usedSensors() const
//{
//	if (mType == robots::enums::sensorType::unused) {
//		return QList<SensorPortPair>();
//	}
//	robots::enums::inputPort::InputPortEnum const port = static_cast<robots::enums::inputPort::InputPortEnum>(intProperty("Port") - 1);
//	return QList<SensorPortPair>() << qMakePair(mType, static_cast<int>(port));
//}

void WaitForSensorBlock::timerTimeout()
{
	/// @todo True horror.
	robotParts::Device * const device = mRobotModel.configuration().device(mPort);
	robotParts::ScalarSensor *sensor = dynamic_cast<robotParts::ScalarSensor *>(device);
	if (sensor) {
		sensor->read();
	}
}

void WaitForSensorBlock::stop()
{
	/// @todo True horror.
	robotParts::Device * const device = mRobotModel.configuration().device(mPort);
	robotParts::ScalarSensor * const sensor = dynamic_cast<robotParts::ScalarSensor *>(device);

	if (sensor) {
		disconnect(sensor, &robotParts::ScalarSensor::newData, this, &WaitForSensorBlock::responseSlot);
		disconnect(sensor, &robotParts::AbstractSensor::failure, this, &WaitForSensorBlock::failureSlot);
	}

	WaitBlock::stop();
}

void WaitForSensorBlock::stopActiveTimerInBlock()
{
	/// @todo True horror.
	robotParts::Device * const device = mRobotModel.configuration().device(mPort);
	robotParts::ScalarSensor * const sensor = dynamic_cast<robotParts::ScalarSensor *>(device);

	if (sensor) {
		disconnect(sensor, &robotParts::ScalarSensor::newData, this, &WaitForSensorBlock::responseSlot);
		disconnect(sensor, &robotParts::AbstractSensor::failure, this, &WaitForSensorBlock::failureSlot);
	}

	WaitBlock::stopActiveTimerInBlock();
}
