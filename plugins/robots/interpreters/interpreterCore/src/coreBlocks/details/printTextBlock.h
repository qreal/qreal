#pragma once

#include <interpreterBase/blocksBase/common/displayBlock.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for print text block.
/// Outputs the given string in specified in help format to the robot`s screen into specified coordinates.
class PrintTextBlock : public interpreterBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit PrintTextBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(interpreterBase::robotModel::robotParts::Display &display) override;
};

}
}
}
