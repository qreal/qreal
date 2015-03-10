#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace ev3 {
namespace blocks {
namespace details {

class DrawRectBlock : public kitBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit DrawRectBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;
};

}
}
}
