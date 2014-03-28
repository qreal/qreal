#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForLightSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForLightSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForLightSensorBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
//	virtual robotParts::Sensor *sensor() const;
	QString name() const override;
};
}
}
}
