#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace trik {
namespace blocks {
namespace details {

class SmileBlock : public kitBase::blocksBase::common::DisplayBlock
{
public:
	SmileBlock(kitBase::robotModel::RobotModelInterface &robotModel, bool isSmileSad);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;

	bool mIsSmileSad;
};

}
}
}
