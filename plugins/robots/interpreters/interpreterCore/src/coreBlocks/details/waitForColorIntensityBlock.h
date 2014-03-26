#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForColorIntensityBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	explicit WaitForColorIntensityBlock(RobotModel * const robotModel);
	virtual ~WaitForColorIntensityBlock() {}

protected slots:
	void responseSlot(int reading);

};

}
}
}
