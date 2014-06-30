#pragma once

#include <interpreterBase/blocksBase/common/displayBlock.h>

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class DrawCircleBlock : public interpreterBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawCircleBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(interpreterBase::robotModel::robotParts::Display &display) override;
};

}
}
}
