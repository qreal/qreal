#include "waitForSensorBlock.h"

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

using namespace interpreterCore::coreBlocks::details;

WaitForSensorBlock::WaitForSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
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
	/// @todo Move port search code into a common place
	for (interpreterBase::robotModel::PortInfo const &portInfo : mRobotModel.availablePorts()) {
		if (portInfo.name() == port || portInfo.nameAliases().contains(port)) {
			interpreterBase::robotModel::robotParts::Device *device
					= mRobotModel.configuration().device(portInfo
							, interpreterBase::robotModel::ConfigurationInterface::output);
			interpreterBase::robotModel::robotParts::ScalarSensor *sensor
					= dynamic_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);
			if (sensor) {
				mPort = portInfo;
				connect(sensor, &interpreterBase::robotModel::robotParts::ScalarSensor::newData
						, this, &WaitForSensorBlock::responseSlot);
				connect(sensor, &interpreterBase::robotModel::robotParts::AbstractSensor::failure
						, this, &WaitForSensorBlock::failureSlot);
				sensor->read();
				mActiveWaitingTimer.start();
			} else {
				mActiveWaitingTimer.stop();
				error(tr("%1 is not configured on port %2").arg(name(), portInfo.name()));
				return;
			}
		}
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
	interpreterBase::robotModel::robotParts::Device *device
			= mRobotModel.configuration().device(mPort
					, interpreterBase::robotModel::ConfigurationInterface::output);

	interpreterBase::robotModel::robotParts::ScalarSensor *sensor
			= static_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);

	if (sensor) {
		sensor->read();
	}
}

void WaitForSensorBlock::stop()
{
	/// @todo True horror.
	interpreterBase::robotModel::robotParts::Device *device
			= mRobotModel.configuration().device(mPort
					, interpreterBase::robotModel::ConfigurationInterface::output);

	interpreterBase::robotModel::robotParts::ScalarSensor *sensor
			= static_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);

	if (sensor) {
		disconnect(sensor, &interpreterBase::robotModel::robotParts::ScalarSensor::newData
				, this, &WaitForSensorBlock::responseSlot);

		disconnect(sensor, &interpreterBase::robotModel::robotParts::AbstractSensor::failure
				, this, &WaitForSensorBlock::failureSlot);
	}

	WaitBlock::stop();
}

void WaitForSensorBlock::stopActiveTimerInBlock()
{
	/// @todo True horror.
	interpreterBase::robotModel::robotParts::Device *device
			= mRobotModel.configuration().device(mPort
					, interpreterBase::robotModel::ConfigurationInterface::output);

	interpreterBase::robotModel::robotParts::ScalarSensor *sensor
			= static_cast<interpreterBase::robotModel::robotParts::ScalarSensor *>(device);

	if (sensor) {
		disconnect(sensor, &interpreterBase::robotModel::robotParts::ScalarSensor::newData
				, this, &WaitForSensorBlock::responseSlot);

		disconnect(sensor, &interpreterBase::robotModel::robotParts::AbstractSensor::failure
				, this, &WaitForSensorBlock::failureSlot);
	}

	WaitBlock::stopActiveTimerInBlock();
}
