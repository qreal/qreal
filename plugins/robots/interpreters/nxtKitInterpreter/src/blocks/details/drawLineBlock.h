#pragma once

#include "displayBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class DrawLineBlock : public DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawLineBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::NxtDisplay &display) override;
};

}
}
}
