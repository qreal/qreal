#include "waitBlock.h"

using namespace qReal::interpreters::robots::details::blocks;

WaitBlock::WaitBlock(details::RobotModel * const robotModel)
	: mRobotModel(robotModel)
{
	mActiveWaitingTimer.setInterval(20);
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitBlock::processResponce(int reading, int targetValue)
{
	QString const sign = stringProperty("Sign");
	if (sign == QString::fromUtf8("равно") && reading == targetValue) {
		stop();
	}
	if (sign == QString::fromUtf8("больше") && reading > targetValue) {
		stop();
	}
	if (sign == QString::fromUtf8("меньше") && reading < targetValue) {
		stop();
	}
	if (sign == QString::fromUtf8("не меньше") && reading >= targetValue) {
		stop();
	}
	if (sign == QString::fromUtf8("не больше") && reading <= targetValue) {
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
