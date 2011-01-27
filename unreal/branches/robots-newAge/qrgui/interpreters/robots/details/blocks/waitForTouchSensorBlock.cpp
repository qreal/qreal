#include "waitForTouchSensorBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(RobotModel const * const robotModel)
	: mTouchSensor(robotModel->touchSensor(inputPort::port1))
{
	// There is about 30 ms latency within robot bluetooth chip, so it is useless to
	// read sensor too frequently.
	mActiveWaitingTimer.setInterval(100);

	connect(mTouchSensor, SIGNAL(response(int)), this, SLOT(response(int)));
	connect(mTouchSensor, SIGNAL(failure()), this, SLOT(failure()));
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitForTouchSensorBlock::run()
{
	mTouchSensor->read();
	mActiveWaitingTimer.start();
}

void WaitForTouchSensorBlock::response(int reading)
{
	if (reading == 1) {
		mActiveWaitingTimer.stop();
		emit done(mNextBlock);
	}
}

void WaitForTouchSensorBlock::failure()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitForTouchSensorBlock::timerTimeout()
{
	mTouchSensor->read();
}
