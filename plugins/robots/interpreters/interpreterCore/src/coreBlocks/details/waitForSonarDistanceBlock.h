#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForSonarDistanceBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForSonarDistanceBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForSonarDistanceBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
