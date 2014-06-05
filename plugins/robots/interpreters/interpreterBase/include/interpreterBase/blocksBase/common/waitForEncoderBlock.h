#pragma once

#include <interpreterBase/blocksBase/common/waitBlock.h>

namespace interpreterBase {
namespace robotModel {
namespace robotParts {
class EncoderSensor;
}
}

namespace blocksBase {
namespace common {

/// An interpreter`s implementation for the encoder`s wait block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForEncoderBlock : public interpreterBase::blocksBase::common::WaitBlock
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
	interpreterBase::robotModel::DeviceInfo device() const override;

	interpreterBase::robotModel::robotParts::EncoderSensor *mEncoderSensor;  // Does not take ownership
};

}
}
}
