#pragma once

#include "displayBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class DrawRectBlock : public DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawRectBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::NxtDisplay &display) override;
};

}
}
}
