#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace trik {
namespace blocks {
namespace details {

class DrawArcBlock : public kitBase::blocksBase::common::DisplayBlock
{
public:
	DrawArcBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;
};

}
}
}
