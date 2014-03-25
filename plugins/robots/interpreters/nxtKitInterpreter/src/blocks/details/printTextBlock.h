#pragma once

#include "displayBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class PrintTextBlock : public DisplayBlock
{
	Q_OBJECT

public:
	explicit PrintTextBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::NxtDisplay &display) override;
};

}
}
}
