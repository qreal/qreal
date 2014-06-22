#pragma once

#include "waitForSensorBlock.h"

namespace interpreterBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for sound sensor" block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForSoundSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForSoundSensorBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

protected slots:
	void responseSlot(int reading) override;

protected:
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
