#include "kitBase/blocksBase/common/engineCommandBlock.h"

#include <kitBase/robotModel/robotModelUtils.h>

using namespace kitBase::blocksBase::common;
using namespace kitBase::robotModel;

EngineCommandBlock::EngineCommandBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

QMap<PortInfo, DeviceInfo> EngineCommandBlock::usedDevices()
{
	QMap<PortInfo, DeviceInfo> result;
	for (robotParts::Motor * const motor : parsePorts<robotParts::Motor>(ReportErrors::doNotReport)) {
		result[motor->port()] = motor->deviceInfo();
	}

	return result;
}

void EngineCommandBlock::timeout()
{
	emit done(mNextBlockId);
}
