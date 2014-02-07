#include "waitBlock.h"

using namespace interpreterCore::coreBlocks::details;

WaitBlock::WaitBlock(interpreterBase::robotModel::RobotModelInterface * const robotModel)
	: mRobotModel(robotModel)
{
	mActiveWaitingTimer.setInterval(20);
	connect(&mActiveWaitingTimer, &QTimer::timeout, this, &WaitBlock::timerTimeout);
}

void WaitBlock::setFailedStatus()
{
	Block::setFailedStatus();
	stopActiveTimerInBlock();
}

void WaitBlock::processResponce(int reading, int targetValue)
{
	QString const sign = stringProperty("Sign");
	if (sign == QString::fromUtf8("равно") && reading == targetValue) {
		stop();
	} else if (sign == QString::fromUtf8("больше") && reading > targetValue) {
		stop();
	} else if (sign == QString::fromUtf8("меньше") && reading < targetValue) {
		stop();
	} else if (sign == QString::fromUtf8("не меньше") && reading >= targetValue) {
		stop();
	} else if (sign == QString::fromUtf8("не больше") && reading <= targetValue) {
		stop();
	}
}

void WaitBlock::stop()
{
	mActiveWaitingTimer.stop();
	emit done(mNextBlockId);
}

void WaitBlock::failureSlot()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitBlock::stopActiveTimerInBlock()
{
	mActiveWaitingTimer.stop();
}
