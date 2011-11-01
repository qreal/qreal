#include "waitForEncoderBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForEncoderBlock::WaitForEncoderBlock(details::RobotModel * const robotModel)
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
	QString const port = stringProperty("Port");
	if (port.trimmed().toUpper() == "A")
		mEncoderSensor = &mRobotModel->encoderA();
	else if (port.trimmed().toUpper() == "B")
		mEncoderSensor = &mRobotModel->encoderB();
	else if (port.trimmed().toUpper() == "C")
		mEncoderSensor = &mRobotModel->encoderC();
	if (!mEncoderSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Encoder sensor is not configured on this port ")); // поправить имя ошибки
		return;
	}

	connect(mEncoderSensor->encoderImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)));
	connect(mEncoderSensor->encoderImpl(), SIGNAL(failure()), this, SLOT(failureSlot()));

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
	int const tachoLimit = evaluate("TachoLimit").toInt();
	if (reading >= tachoLimit)
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

void WaitForEncoderBlock::stopActiveTimerInBlock()
{
	mActiveWaitingTimer.stop();
}
