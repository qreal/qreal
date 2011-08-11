#include "nullificationEncoderBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

NullificationEncoderBlock::NullificationEncoderBlock(RobotModel *const robotModel)
	:  mEncoderSensor(NULL)
	,mRobotModel(robotModel)
{
}

void NullificationEncoderBlock::run()
{
	QString const port = stringProperty("Port");
	if (port.trimmed().toUpper() == "A")
		mEncoderSensor = &mRobotModel->encoderA();
	else if (port.trimmed().toUpper() == "B")
		mEncoderSensor = &mRobotModel->encoderB();
	else if (port.trimmed().toUpper() == "C")
		mEncoderSensor = &mRobotModel->encoderC();
	if (!mEncoderSensor) {
		error(tr("Wrong port to nullificate Encoder"));
		return;
	}
	mEncoderSensor->nullificate();
	emit done(mNextBlock);
}
