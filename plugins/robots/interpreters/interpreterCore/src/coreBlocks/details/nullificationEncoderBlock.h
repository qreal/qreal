#pragma once

#include <interpreterBase/blocksBase/block.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class NullificationEncoderBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit NullificationEncoderBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);

	void run() override;

private:
	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
