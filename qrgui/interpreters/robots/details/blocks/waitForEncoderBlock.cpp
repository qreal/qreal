#include "waitForEncoderBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace details::blocks;

WaitForEncoderBlock::WaitForEncoderBlock(details::RobotModel const * const robotModel)
	: mEncoderSensor(NULL)
	, mRobotModel(robotModel)
{
	// There is about 30 ms latency within robot bluetooth chip, so it is useless to
	// read sensor too frequently.
	mActiveWaitingTimer.setInterval(100);

	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitForEncoderBlock::run()
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	mEncoderSensor = mRobotModel->encoderSensor(port);

	if (!mEncoderSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Encoder sensor is not configured on this port"));
		return;
	}

	connect(mEncoderSensor->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)));
	connect(mEncoderSensor->sensorImpl(), SIGNAL(failure()), this, SLOT(failureSlot()));

	mEncoderSensor->read();
	mActiveWaitingTimer.start();
}

void WaitForEncoderBlock::stop()
{
	mActiveWaitingTimer.stop();
	emit done(mNextBlock);
}

void WaitForEncoderBlock::responseSlot(int reading)
{
	int const motorLimit = evaluate("MotorLimit").toInt();
	if (reading >= motorLimit)
		stop();
}

void WaitForEncoderBlock::failureSlot()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitForEncoderBlock::timerTimeout()
{
	mEncoderSensor->read();
}

QList<Block::SensorPortPair> WaitForEncoderBlock::usedSensors() const
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	return QList<SensorPortPair>() << qMakePair(sensorType::encoder, static_cast<int>(port));
}
