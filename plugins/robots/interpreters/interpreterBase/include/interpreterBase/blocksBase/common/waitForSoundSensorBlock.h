#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

class ROBOTS_INTERPRETER_BASE_EXPORT WaitForSoundSensorBlock : public WaitForSensorBlock
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
