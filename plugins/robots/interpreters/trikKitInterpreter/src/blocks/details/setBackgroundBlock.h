#pragma once

#include <interpreterBase/blocksBase/common/displayBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SetBackgroundBlock : public interpreterBase::blocksBase::common::DisplayBlock
{
public:
	explicit SetBackgroundBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(interpreterBase::robotModel::robotParts::Display &display) override;
};

}
}
}
