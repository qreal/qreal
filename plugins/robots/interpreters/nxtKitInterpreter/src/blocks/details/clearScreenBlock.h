#pragma once

#include "displayBlock.h"

namespace nxtKitInterpreter {
namespace blocks {
namespace details {

class ClearScreenBlock : public DisplayBlock
{
	Q_OBJECT

public:
	explicit ClearScreenBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(robotModel::parts::NxtDisplay &display) override;
};

}
}
}
