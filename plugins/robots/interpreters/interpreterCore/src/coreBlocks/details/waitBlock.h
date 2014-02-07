#pragma once

#include <interpreterBase/blocksBase/block.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

namespace interpreterCore {
namespace coreBlocks {
namespace details {

/// @todo Move abstract blocks to interpreterBase::blocksBase
class WaitBlock : public interpreterBase::blocksBase::Block
{
	Q_OBJECT

public:
	explicit WaitBlock(interpreterBase::robotModel::RobotModelInterface &robotModel);
	~WaitBlock() override;

	void setFailedStatus() override;
	virtual void stopActiveTimerInBlock();

protected slots:
	virtual void failureSlot();
	virtual void timerTimeout() = 0;

protected:
	void processResponce(int reading, int targetValue);
	virtual void stop();

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;
	QTimer mActiveWaitingTimer;
};

}
}
}
