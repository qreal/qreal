#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// @todo Move abstract blocks to interpreterBase::blocksBase
class WaitForSensorBlock : public interpreterBase::blocksBase::common::WaitBlock
{
	Q_OBJECT

public:
	explicit WaitForSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel
//			, robots::enums::sensorType::SensorTypeEnum type
			);

	~WaitForSensorBlock() override;

	void run() override;
//	virtual QList<Block::SensorPortPair> usedSensors() const;
	void stopActiveTimerInBlock() override;

protected slots:
	virtual void responseSlot(int reading) = 0;
	void timerTimeout() override;

protected:
//	virtual robotParts::Sensor *sensor() const = 0;
	virtual QString name() const = 0;
	void stop() override;

//	robots::enums::sensorType::SensorTypeEnum mType;
	interpreterBase::robotModel::PortInfo mPort;
};

}
}
}
