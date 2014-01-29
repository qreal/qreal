#pragma once

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class InitialBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit InitialBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	virtual void run();

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
