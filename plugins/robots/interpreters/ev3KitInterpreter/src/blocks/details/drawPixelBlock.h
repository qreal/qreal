#pragma once

#include <interpreterBase/blocksBase/common/displayBlock.h>

namespace ev3KitInterpreter {
namespace blocks {
namespace details {

class DrawPixelBlock : public interpreterBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawPixelBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(interpreterBase::robotModel::robotParts::Display &display) override;
};

}
}
}
