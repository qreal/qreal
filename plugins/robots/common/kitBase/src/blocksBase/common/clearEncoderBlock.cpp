#include "kitBase/blocksBase/common/clearEncoderBlock.h"

#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace kitBase::blocksBase::common;
using namespace kitBase::robotModel;

ClearEncoderBlock::ClearEncoderBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void ClearEncoderBlock::run()
{
	for (robotParts::EncoderSensor * const encoder : parsePorts()) {
		encoder->nullify();
	}

	emit done(mNextBlockId);
}

QMap<PortInfo, DeviceInfo> ClearEncoderBlock::usedDevices()
{
	QMap<PortInfo, DeviceInfo> result;
	for (robotParts::EncoderSensor * const encoder : parsePorts()) {
		result[encoder->port()] = encoder->deviceInfo();
	}

	return result;
}

QList<robotParts::EncoderSensor *> ClearEncoderBlock::parsePorts() const
{
	QList<robotParts::EncoderSensor *> result;
	for (const QString &port : stringProperty("Ports").split(',', QString::SkipEmptyParts)) {
		robotParts::EncoderSensor * const encoder = RobotModelUtils::findDevice<robotParts::EncoderSensor>(
				mRobotModel, port.trimmed());

		if (encoder) {
			result << encoder;
		}
	}

	return result;
}
