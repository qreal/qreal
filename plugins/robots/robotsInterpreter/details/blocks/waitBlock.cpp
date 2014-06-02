#include "waitBlock.h"

using namespace qReal::interpreters::robots::details::blocks;

WaitBlock::WaitBlock(details::RobotModel * const robotModel)
	: mRobotModel(robotModel)
{
	mActiveWaitingTimer.setInterval(20);
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitBlock::setFailedStatus()
{
	Block::setFailedStatus();
	stopActiveTimerInBlock();
}

void WaitBlock::processResponce(int reading, int targetValue)
{
	QString const sign = stringProperty("Sign");
	if (sign == "equals" && reading == targetValue) {
		stop();
	} else if (sign == "greater" && reading > targetValue) {
		stop();
	} else if (sign == "less" && reading < targetValue) {
		stop();
	} else if (sign == "notLess" && reading >= targetValue) {
		stop();
	} else if (sign == "notGreater" && reading <= targetValue) {
		stop();
	}
}

void WaitBlock::stop()
{
	mActiveWaitingTimer.stop();
	emit done(mNextBlock);
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
