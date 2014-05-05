#pragma once

#include "waitForColorSensorBlockBase.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

class ROBOTS_INTERPRETER_BASE_EXPORT WaitForColorIntensityBlock : public WaitForColorSensorBlockBase
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
