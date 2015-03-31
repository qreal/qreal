#pragma once

#include <kitBase/blocksBase/common/displayBlock.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// Interpreter`s implementation for clear screen block. Clears everything on robot`s screen.
class ClearScreenBlock : public kitBase::blocksBase::common::DisplayBlock
{
	Q_OBJECT

public:
	explicit ClearScreenBlock(kitBase::robotModel::RobotModelInterface &robotModel);

private:
	void doJob(kitBase::robotModel::robotParts::Display &display) override;
};

}
}
}
