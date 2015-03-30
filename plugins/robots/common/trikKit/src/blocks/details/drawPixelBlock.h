#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace trik {
namespace blocks {
namespace details {

class DrawPixelBlock : public kitBase::blocksBase::common::DisplayBlock
{
public:
	DrawPixelBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;
};

}
}
}
