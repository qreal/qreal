#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>

namespace interpreterBase {
namespace robotModel {
namespace robotParts {
class EncoderSensor;
}
}
}

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class WaitForEncoderBlock : public interpreterBase::blocksBase::common::WaitBlock
{
	Q_OBJECT

public:
	explicit WaitForEncoderBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitForEncoderBlock() override;

	void run() override;

private slots:
	void responseSlot(int reading);
	void timerTimeout() override;

private:
	interpreterBase::robotModel::robotParts::EncoderSensor *mEncoderSensor;  // Does not take ownership
};

}
}
}
