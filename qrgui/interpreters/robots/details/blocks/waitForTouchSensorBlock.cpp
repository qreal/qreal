#include "waitForTouchSensorBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace details::blocks;

WaitForTouchSensorBlock::WaitForTouchSensorBlock(details::RobotModel const * const robotModel)
	: mTouchSensor(NULL)
	, mRobotModel(robotModel)
{
	// There is about 30 ms latency within robot bluetooth chip, so it is useless to
	// read sensor too frequently.
	mActiveWaitingTimer.setInterval(100);

	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitForTouchSensorBlock::run()
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	mTouchSensor = mRobotModel->touchSensor(port);

	if (!mTouchSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Touch sensor is not configured on this port"));
		return;
	}

	connect(mTouchSensor->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)));
	connect(mTouchSensor->sensorImpl(), SIGNAL(failure()), this, SLOT(failureSlot()));

	mTouchSensor->read();
	mActiveWaitingTimer.start();
}

void WaitForTouchSensorBlock::responseSlot(int reading)
{
	if (reading == 1) {
		mActiveWaitingTimer.stop();
		emit done(mNextBlock);
	}
}

void WaitForTouchSensorBlock::failureSlot()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitForTouchSensorBlock::timerTimeout()
{
	mTouchSensor->read();
}

QList<Block::SensorPortPair> WaitForTouchSensorBlock::usedSensors() const
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	return QList<SensorPortPair>() << qMakePair(sensorType::touchBoolean, static_cast<int>(port));
}

void WaitForTouchSensorBlock::stopActiveTimerInBlock()
{
	mActiveWaitingTimer.stop();
}
