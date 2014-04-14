#pragma once

#include "waitForSensorBlock.h"

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForSoundSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForSoundSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForSoundSensorBlock() override;

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
