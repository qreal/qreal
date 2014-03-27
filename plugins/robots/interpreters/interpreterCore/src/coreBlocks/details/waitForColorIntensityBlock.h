#pragma once

#include "waitForColorSensorBlockBase.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForColorIntensityBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	explicit WaitForColorIntensityBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForColorIntensityBlock() override;

protected slots:
	void responseSlot(int reading) override;
};

}
}
}
