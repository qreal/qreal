#pragma once

#include <interpreterBase/blocksBase/block.h>

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

class NullificationEncoderBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit NullificationEncoderBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;

	QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> usedSensors() const override;

private:
	QList<interpreterBase::robotModel::robotParts::EncoderSensor *> parsePorts() const;

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
