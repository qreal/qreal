#pragma once

#include <kitBase/blocksBase/common/engineCommandBlock.h>

namespace ev3 {
namespace blocks {
namespace details {

class Ev3EnginesBackwardBlock : public kitBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit Ev3EnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
