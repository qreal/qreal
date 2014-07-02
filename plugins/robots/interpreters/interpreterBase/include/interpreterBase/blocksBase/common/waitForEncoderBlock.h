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

/// An interpreter`s implementation for "wait for the encoder" block.
/// @todo Inherit it from WaitForSensorBlock?
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForEncoderBlock : public WaitBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForEncoderBlock(robotModel::RobotModelInterface &robotModel);

	void run() override;

private slots:
	/// Called when new data from encoder ready, checks it against "TachoLimit" property.
	void responseSlot(int reading);

	void timerTimeout() override;

private:
	robotModel::DeviceInfo device() const override;

	robotModel::robotParts::EncoderSensor *mEncoderSensor;  // Does not have ownership
};

}
}
}
