#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

class ROBOTS_INTERPRETER_BASE_EXPORT WaitForColorSensorBlockBase : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForColorSensorBlockBase(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForColorSensorBlockBase() override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
