#include "waitForTouchSensorBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(robotParts::TouchSensor &touchSensor)
	: mTouchSensor(touchSensor)
{
	// There is about 30 ms latency within robot bluetooth chip, so it is useless to
	// read sensor too frequently.
	mActiveWaitingTimer.setInterval(100);

	connect(&touchSensor, SIGNAL(response(int)), this, SLOT(response(int)));
	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitForTouchSensorBlock::run()
{
	mTouchSensor.read();
	mActiveWaitingTimer.start();
}

void WaitForTouchSensorBlock::response(int reading)
{
	qDebug() << "Touch sensor reading:" << reading;
	if (reading == 1)
		emit done(mNextBlock);
}

void WaitForTouchSensorBlock::timerTimeout()
{
	mTouchSensor.read();
}
