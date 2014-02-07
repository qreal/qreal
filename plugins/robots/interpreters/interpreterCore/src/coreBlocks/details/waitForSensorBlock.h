#pragma once

#include "waitBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// @todo Move abstract blocks to interpreterBase::blocksBase
class WaitForSensorBlock : public WaitBlock
{
	Q_OBJECT

public:
	WaitForSensorBlock(interpreterBase::robotModel::RobotModelInterface * const robotModel
//			, robots::enums::sensorType::SensorTypeEnum type
			);

	~WaitForSensorBlock() override;

	void run() override;
//	virtual QList<Block::SensorPortPair> usedSensors() const;
	void stopActiveTimerInBlock() override;

protected slots:
	virtual void responseSlot(int reading) = 0;
	virtual void timerTimeout();

protected:
//	virtual robotParts::Sensor *sensor() const = 0;
	virtual QString name() const = 0;
	virtual void stop();

//	robots::enums::sensorType::SensorTypeEnum mType;
//	robots::enums::inputPort::InputPortEnum mPort;
};

}
}
}
