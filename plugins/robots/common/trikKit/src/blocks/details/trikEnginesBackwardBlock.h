#pragma once

#include <kitBase/blocksBase/common/engineCommandBlock.h>

namespace trik {
namespace blocks {
namespace details {

class TrikEnginesBackwardBlock : public kitBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit TrikEnginesBackwardBlock(kitBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
