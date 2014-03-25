#pragma once

#include "displayBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class DrawCircleBlock : public DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawCircleBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::NxtDisplay &display) override;
};

}
}
}
