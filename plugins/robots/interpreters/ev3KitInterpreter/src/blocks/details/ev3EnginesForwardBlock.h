#pragma once

#include <interpreterBase/blocksBase/common/engineCommandBlock.h>

namespace ev3KitInterpreter {
namespace blocks {
namespace details {

class Ev3EnginesForwardBlock : public interpreterBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit Ev3EnginesForwardBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
