#pragma once

#include <kitBase/blocksBase/common/engineCommandBlock.h>

namespace trik {
namespace blocks {
namespace details {

class TrikEnginesForwardBlock : public kitBase::blocksBase::common::EngineCommandBlock
{
	Q_OBJECT

public:
	explicit TrikEnginesForwardBlock(kitBase::robotModel::RobotModelInterface &robotModel);

	void run() override;
};

}
}
}
