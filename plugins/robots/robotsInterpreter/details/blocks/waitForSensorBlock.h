#pragma once

#include "waitBlock.h"
#include "../robotParts/robotModel.h"

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace blocks
{

class WaitForSensorBlock : public WaitBlock
{
	Q_OBJECT

public:
	WaitForSensorBlock(RobotModel * const robotModel, sensorType::SensorTypeEnum type);
	virtual ~WaitForSensorBlock();

	virtual void run();
	virtual QList<Block::SensorPortPair> usedSensors() const;
	virtual void stopActiveTimerInBlock();

protected slots:
	virtual void responseSlot(int reading) = 0;
	virtual void timerTimeout();

protected:
	virtual robotParts::Sensor *sensor() const = 0;
	virtual QString name() const = 0;
	virtual void stop();

	sensorType::SensorTypeEnum mType;
	inputPort::InputPortEnum mPort;
};

}
}
}
}
}
