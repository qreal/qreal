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


class ROBOTS_INTERPRETER_BASE_EXPORT ClearEncoderBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit ClearEncoderBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;

	QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> usedSensors() const override;

private:
	QList<interpreterBase::robotModel::robotParts::EncoderSensor *> parsePorts() const;

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
