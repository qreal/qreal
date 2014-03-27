#pragma once

#include "waitForColorSensorBlockBase.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForColorBlock : public WaitForColorSensorBlockBase
{
	Q_OBJECT

public:
	explicit WaitForColorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForColorBlock() override;

protected slots:
	void responseSlot(int reading) override;
};

}
}
}
