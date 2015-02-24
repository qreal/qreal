#include "interpreterBase/blocksBase/common/engineCommandBlock.h"

#include <interpreterBase/robotModel/robotModelUtils.h>

using namespace interpreterBase::blocksBase::common;
using namespace interpreterBase::robotModel;

EngineCommandBlock::EngineCommandBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

QMap<PortInfo, DeviceInfo> EngineCommandBlock::usedDevices()
{
	QMap<PortInfo, DeviceInfo> result;
	for (const PortInfo &port : parsePorts(ReportErrors::doNotReport)) {
		result[port] = DeviceInfo::create<robotParts::Motor>();
	}

	return result;
}

void EngineCommandBlock::timeout()
{
	emit done(mNextBlockId);
}

QList<PortInfo> EngineCommandBlock::parsePorts(ReportErrors reportErrors)
{
	QList<PortInfo> result;
	const QStringList ports = eval<QStringList>("Ports", reportErrors);

	for (const QString &port : ports) {
		PortInfo const portInfo = robotModel::RobotModelUtils::findPort(mRobotModel, port.trimmed(), output);
		if (portInfo.isValid()) {
			result << portInfo;
		}
	}

	return result;
}
