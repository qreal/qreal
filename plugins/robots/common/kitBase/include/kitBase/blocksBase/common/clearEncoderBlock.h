#pragma once

#include <kitBase/blocksBase/robotsBlock.h>

namespace kitBase {

namespace robotModel {
namespace robotParts {
class EncoderSensor;
}
}

namespace blocksBase {
namespace common {

/// An interpreter's implementation for "clear encoder" block.
class ROBOTS_KIT_BASE_EXPORT ClearEncoderBlock : public RobotsBlock
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
