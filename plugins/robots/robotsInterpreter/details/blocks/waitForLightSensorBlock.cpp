#include "waitForLightSensorBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForLightSensorBlock::WaitForLightSensorBlock(details::RobotModel const * const robotModel)
	: mLightSensor(NULL)
	, mRobotModel(robotModel)
{
	// There is about 30 ms latency within robot bluetooth chip, so it is useless to
	// read sensor too frequently.
	mActiveWaitingTimer.setInterval(100);

	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitForLightSensorBlock::run()
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	mLightSensor = mRobotModel->lightSensor(port);

	if (!mLightSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Light sensor is not configured on this port"));
		return;
	}

	connect(mLightSensor->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)));
	connect(mLightSensor->sensorImpl(), SIGNAL(failure()), this, SLOT(failureSlot()));

	mLightSensor->read();
	mActiveWaitingTimer.start();
}

void WaitForLightSensorBlock::responseSlot(int reading)
{
	int const targetPercents = evaluate("Percents").toInt();

	QString const sign = stringProperty("Sign");
	if ((sign == "равно") && (reading != targetPercents)) {
		stop();
	}
	if ((sign == "больше") && (reading <= targetPercents)) {
		stop();
	}
	if ((sign == "меньше") && (reading >= targetPercents)) {
		stop();
	}
	if ((sign == "не меньше") && (reading < targetPercents)) {
		stop();
	}
	if ((sign == "не больше") && (reading > targetPercents)) {
		stop();
	}

	if (targetPercents < reading) {
		stop();
	}
}

void WaitForLightSensorBlock::stop()
{
	mActiveWaitingTimer.stop();
	emit done(mNextBlock);
}

void WaitForLightSensorBlock::failureSlot()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitForLightSensorBlock::timerTimeout()
{
	mLightSensor->read();
}

QList<Block::SensorPortPair> WaitForLightSensorBlock::usedSensors() const
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	return QList<SensorPortPair>() << qMakePair(sensorType::light, static_cast<int>(port));
}

void WaitForLightSensorBlock::stopActiveTimerInBlock()
{
	mActiveWaitingTimer.stop();
}
