#pragma once

#include <interpreterBase/blocksBase/common/displayBlock.h>

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class DrawLineBlock : public interpreterBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawLineBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(interpreterBase::robotModel::robotParts::Display &display) override;
};

}
}
}
