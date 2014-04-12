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
	for (robotParts::EncoderSensor * const encoder : parsePorts()) {
		encoder->nullificate();
	}

	emit done(mNextBlockId);
}

QMap<PortInfo, DeviceInfo> NullificationEncoderBlock::usedSensors() const
{
	QMap<PortInfo, DeviceInfo> result;
	for (robotParts::EncoderSensor * const encoder : parsePorts()) {
		result[encoder->port()] = encoder->deviceInfo();
	}

	return result;
}

QList<robotParts::EncoderSensor *> NullificationEncoderBlock::parsePorts() const
{
	QList<robotParts::EncoderSensor *> result;
	for (QString const &port : stringProperty("Ports").split(',', QString::SkipEmptyParts)) {
		robotParts::EncoderSensor * const encoder = RobotModelUtils::findDevice<robotParts::EncoderSensor>(
				mRobotModel, port);
		if (encoder) {
			result << encoder;
		}
	}

	return result;
}
