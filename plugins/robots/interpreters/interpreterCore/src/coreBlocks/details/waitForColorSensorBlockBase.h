#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForColorSensorBlockBase : public WaitForSensorBlock
{
	Q_OBJECT

public:
	WaitForColorSensorBlockBase(RobotModel * const robotModel, robots::enums::sensorType::SensorTypeEnum type);
	virtual ~WaitForColorSensorBlockBase() {}

protected:
	virtual robotParts::Sensor *sensor() const;
	virtual QString name() const;
};

}
}
}
