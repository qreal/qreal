#include "waitForEncoderBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForEncoderBlock::WaitForEncoderBlock(details::RobotModel * const robotModel)
	: WaitBlock(robotModel)
	, mEncoderSensor(NULL)
{
}

void WaitForEncoderBlock::run()
{
	QString const port = stringProperty("Port");
	if (port.trimmed().toUpper() == "A") {
		mEncoderSensor = &mRobotModel->encoderA();
	} else if (port.trimmed().toUpper() == "B") {
		mEncoderSensor = &mRobotModel->encoderB();
	} else if (port.trimmed().toUpper() == "C") {
		mEncoderSensor = &mRobotModel->encoderC();
	}

	if (!mEncoderSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Encoder sensor is not configured on this port "));
		return;
	}

	connect(mEncoderSensor->encoderImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)));
	connect(mEncoderSensor->encoderImpl(), SIGNAL(failure()), this, SLOT(failureSlot()));

	mEncoderSensor->read();
	mActiveWaitingTimer.start();
}

void WaitForEncoderBlock::responseSlot(int reading)
{
	int const tachoLimit = evaluate("TachoLimit").toInt();
	processResponce(reading, tachoLimit);
}

void WaitForEncoderBlock::timerTimeout()
{
	mEncoderSensor->read();
}
