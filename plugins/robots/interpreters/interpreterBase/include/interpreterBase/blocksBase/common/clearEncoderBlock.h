#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterBase {

namespace robotModel {
namespace robotParts {
class EncoderSensor;
}
}

namespace blocksBase {
namespace common {

/// An interpreter's implementation for "clear encoder" block.
class ROBOTS_INTERPRETER_BASE_EXPORT ClearEncoderBlock : public Block
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit ClearEncoderBlock(robotModel::RobotModelInterface &robotModel);

	void run() override;

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices() override;

private:
	/// Splits value of "Ports" property by "," and finds corresponding encoders in a model.
	QList<robotModel::robotParts::EncoderSensor *> parsePorts() const;

	robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
