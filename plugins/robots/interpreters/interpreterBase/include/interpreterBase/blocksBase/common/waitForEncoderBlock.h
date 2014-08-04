#pragma once

#include <interpreterBase/blocksBase/common/waitForSensorBlock.h>

namespace interpreterBase {
namespace robotModel {
namespace robotParts {
class EncoderSensor;
}
}

namespace blocksBase {
namespace common {

/// An interpreter`s implementation for "wait for the encoder" block.
class ROBOTS_INTERPRETER_BASE_EXPORT WaitForEncoderBlock : public WaitForSensorBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForEncoderBlock(robotModel::RobotModelInterface &robotModel);

private slots:
	/// Called when new data from encoder ready, checks it against "TachoLimit" property.
	void responseSlot(int reading);

private:
	robotModel::DeviceInfo device() const override;
};

}
}
}
