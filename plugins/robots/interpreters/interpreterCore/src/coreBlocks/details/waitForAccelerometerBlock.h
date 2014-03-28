#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForAccelerometerSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForAccelerometerSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForAccelerometerSensorBlock() override;

protected slots:
	virtual void responseSlot(int reading);

protected:
//	virtual robotParts::Sensor *sensor() const;
	virtual QString name() const;
};

}
}
}
