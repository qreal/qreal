#pragma once

#include "waitForSensorBlock.h"

namespace kitBase {
namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for sound sensor" block.
class ROBOTS_KIT_BASE_EXPORT WaitForSoundSensorBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	explicit WaitForSoundSensorBlock(kitBase::robotModel::RobotModelInterface &robotModel);

protected slots:
	void responseSlot(int reading) override;

protected:
	kitBase::robotModel::DeviceInfo device() const override;
};

}
}
}
