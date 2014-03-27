#include "nullificationEncoderBlock.h"

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace qReal;
using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

NullificationEncoderBlock::NullificationEncoderBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void NullificationEncoderBlock::run()
{
	QString const ports = stringProperty("Ports");
	QStringList const splitted = ports.split(',', QString::SkipEmptyParts);

	for (QString const &port : splitted) {
		robotParts::EncoderSensor * const encoder = RobotModelUtils::findDevice<robotParts::EncoderSensor>(
				mRobotModel, port);
		if (encoder) {
			encoder->nullificate();
		}
	}

	emit done(mNextBlockId);
}
