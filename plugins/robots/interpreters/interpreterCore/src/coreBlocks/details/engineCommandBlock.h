#pragma once

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>
#include <interpreterBase/robotModel/robotParts/motor.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// @todo Move abstract blocks to interpreterBase::blocksBase
class EngineCommandBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit EngineCommandBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> usedSensors() const override;

protected slots:
	void timeout();

protected:
	/// Splits 'Port' property for the blocks and returns motor devices on them. Implementation may consider
	/// that devices are non-null.
	QList<interpreterBase::robotModel::robotParts::Motor *> parsePorts() const;

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
