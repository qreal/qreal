#include "waitForEncoderBlock.h"

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

WaitForEncoderBlock::WaitForEncoderBlock(RobotModelInterface &robotModel)
	: WaitBlock(robotModel)
	, mEncoderSensor(nullptr)
{
}

WaitForEncoderBlock::~WaitForEncoderBlock()
{
}

void WaitForEncoderBlock::run()
{
	QString const port = stringProperty("Port");
	mEncoderSensor = RobotModelUtils::findDevice<robotParts::EncoderSensor>(mRobotModel, port);

	if (!mEncoderSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Encoder sensor is not configured on port %1").arg(port));
		return;
	}

	connect(mEncoderSensor, &robotParts::EncoderSensor::newData, this, &WaitForEncoderBlock::responseSlot);
	connect(mEncoderSensor, &robotParts::EncoderSensor::failure, this, &WaitForEncoderBlock::failureSlot);

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
