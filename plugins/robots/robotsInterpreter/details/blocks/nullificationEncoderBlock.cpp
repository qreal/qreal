#include "nullificationEncoderBlock.h"

using namespace qReal;
using namespace interpreters::robots::details::blocks;

NullificationEncoderBlock::NullificationEncoderBlock(RobotModel * const robotModel)
	: mRobotModel(robotModel)
{
}

void NullificationEncoderBlock::run()
{
	QVector<bool> ports(parseEnginePorts());
	if (ports[0]) {
		mRobotModel->encoderA().nullificate();
	}
	if (ports[1]) {
		mRobotModel->encoderB().nullificate();
	}
	if (ports[2]) {
		mRobotModel->encoderC().nullificate();
	}
	emit done(mNextBlock);
}
