#include "engineCommandBlock.h"

#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterCore::coreBlocks::details;
using namespace interpreterBase::robotModel;

EngineCommandBlock::EngineCommandBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

QMap<PortInfo, DeviceInfo> EngineCommandBlock::usedSensors() const
{
	QMap<PortInfo, DeviceInfo> result;
	for (robotParts::Motor * const motor : parsePorts()) {
		result[motor->port()] = motor->deviceInfo();
	}

	return result;
}

void EngineCommandBlock::timeout()
{
	emit done(mNextBlockId);
}

QList<robotParts::Motor *> EngineCommandBlock::parsePorts() const
{
	QList<robotParts::Motor *> result;
	QString const ports = stringProperty("Ports");
	QStringList const splitted = ports.split(',', QString::SkipEmptyParts);

	for (QString const &port : splitted) {
		robotParts::Motor * const motor = RobotModelUtils::findDevice<robotParts::Motor>(mRobotModel, port.trimmed());
		if (motor) {
			result << motor;
		}
	}

	return result;
}
