#pragma once

#include <kitBase/blocksBase/common/engineCommandBlock.h>

namespace nxt {
namespace blocks {
namespace details {

class NxtEnginesBackwardBlock : public kitBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit NxtEnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
