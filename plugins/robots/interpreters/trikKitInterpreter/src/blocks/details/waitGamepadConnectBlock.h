#pragma once

#include <interpreterBase/blocksBase/common/deviceBlock.h>

#include "robotModel/parts/trikGamepadConnectIndicator.h"

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class WaitGamepadConnectBlock : public interpreterBase::blocksBase::common::DeviceBlock<robotModel::parts::TrikSpeaker>
{
public:
	explicit WaitForMotionBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private slots:
	void responseSlot(int reading) override;

private:
	QString port() const override;
	interpreterBase::robotModel::DeviceInfo device() const override;
};

}
}
}
