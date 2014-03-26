#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForColorBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	explicit WaitForColorBlock(RobotModel * const robotModel);
	virtual ~WaitForColorBlock() {}

protected slots:
	virtual void responseSlot(int reading);
};

}
}
}
