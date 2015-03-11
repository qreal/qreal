#pragma once

#include <kitBase/blocksBase/common/engineCommandBlock.h>

namespace nxt {
namespace blocks {
namespace details {

class NxtEnginesForwardBlock : public kitBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit NxtEnginesForwardBlock(kitBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
