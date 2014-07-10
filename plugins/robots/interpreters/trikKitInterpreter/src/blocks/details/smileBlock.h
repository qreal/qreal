#pragma once

#include <interpreterBase/blocksBase/common/displayBlock.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SmileBlock : public interpreterBase::blocksBase::common::DisplayBlock
{
public:
	SmileBlock(interpreterBase::robotModel::RobotModelInterface &robotModel, bool isSmileSad);

private:
	void doJob(interpreterBase::robotModel::robotParts::Display &display) override;

	bool mIsSmileSad;
};

}
}
}
