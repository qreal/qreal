#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace trikKitInterpreter {
namespace blocks {
namespace details {

class SmileBlockBase : public interpreterBase::blocksBase::Block
{
public:
	SmileBlockBase(interpreterBase::robotModel::RobotModelInterface &robotModel, bool isSmileHappy);

private:
	void run() override;

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
	bool mIsSmileHappy;
};

}
}
}
