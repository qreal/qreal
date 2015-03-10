#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace trik {
namespace blocks {
namespace details {

class DrawLineBlock : public kitBase::blocksBase::common::DisplayBlock
{
public:
	DrawLineBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;
};

}
}
}
