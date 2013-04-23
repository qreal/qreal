#pragma once

#include "waitForSensorBlock.h"

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

class WaitForColorSensorBlockBase : public WaitForSensorBlock
{
public:
	WaitForColorSensorBlockBase(RobotModel * const robotModel, sensorType::SensorTypeEnum type);
	virtual ~WaitForColorSensorBlockBase() {}

protected:
	virtual robotParts::Sensor *sensor() const;
	virtual QString name() const;
};

}
}
}
}
}
