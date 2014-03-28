#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForGyroscopeSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForGyroscopeSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForGyroscopeSensorBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
//	virtual robotParts::Sensor *sensor() const;
	QString name() const override;
};
}
}
}
