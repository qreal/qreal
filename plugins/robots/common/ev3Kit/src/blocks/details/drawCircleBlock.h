#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace ev3 {
namespace blocks {
namespace details {

class DrawCircleBlock : public kitBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawCircleBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;
};

}
}
}
