/* Copyright 2013-2016 CyberTech Labs Ltd., Grigorii Zimin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	for (const PortInfo &port : parsePorts(ReportErrors::doNotReport)) {
		result[port] = DeviceInfo::create<robotParts::Motor>();
	}

	return result;
}

robotParts::MotorsAggregator *EngineCommandBlock::findMotorsAggregator() const
{
	return robotModel::RobotModelUtils::findDevice<robotParts::MotorsAggregator>(mRobotModel, "MAll");
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
