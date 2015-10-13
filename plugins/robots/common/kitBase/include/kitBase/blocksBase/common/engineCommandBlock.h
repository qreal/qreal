/* Copyright 2007-2015 QReal Research Group
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

#pragma once

#include <kitBase/blocksBase/robotsBlock.h>
#include <kitBase/robotModel/robotModelInterface.h>
#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/motor.h>

namespace kitBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that work with motors.
class ROBOTS_KIT_BASE_EXPORT EngineCommandBlock : public RobotsBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit EngineCommandBlock(robotModel::RobotModelInterface &robotModel);

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices() override;

protected slots:
	/// @todo Why it is needed and where it is used?
	void timeout();

protected:
	/// Splits 'Port' property for the blocks and returns motor devices on them. Implementation may consider
	/// that devices are non-null.
	template<class MotorType>
	QList<MotorType *> parsePorts(ReportErrors reportErrors = ReportErrors::report)
	{
		QList<MotorType *> result;
		const QList<robotModel::PortInfo> ports = parsePorts(reportErrors);

		for (const robotModel::PortInfo &port : ports) {
			MotorType * const motor = robotModel::RobotModelUtils::findDevice<MotorType>(mRobotModel, port);
			if (motor) {
				result << motor;
			}
		}

		return result;
	}

	/// Non-template method for evaluating 'Port' property into a list of PortInfos.
	QList<robotModel::PortInfo> parsePorts(ReportErrors reportErrors = ReportErrors::report);

	/// @todo Move it to Block maybe?
	robotModel::RobotModelInterface &mRobotModel;
};

}
}
}
