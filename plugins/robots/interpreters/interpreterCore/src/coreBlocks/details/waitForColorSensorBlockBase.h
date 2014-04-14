#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForColorSensorBlockBase : public WaitForSensorBlock
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
