#pragma once

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

class TimerBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit TimerBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~TimerBlock() override;

	void run() override;

private slots:
	void timeout();

private:
	utils::AbstractTimer * const mTimer;  // Takes ownership
};

}
}
}
