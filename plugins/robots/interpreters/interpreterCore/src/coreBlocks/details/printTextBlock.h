#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for print text block.
/// Outputs the given string in specified in help format to the robot`s screen into specified coordinates.
class PrintTextBlock : public kitBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit PrintTextBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;
};

}
}
}
