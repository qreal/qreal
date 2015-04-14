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

#include <kitBase/blocksBase/common/waitBlock.h>

namespace kitBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that wait for sensor.
class ROBOTS_KIT_BASE_EXPORT WaitForSensorBlock : public WaitBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitForSensorBlock(robotModel::RobotModelInterface &robotModel);

	void run() override;

	void stopActiveTimerInBlock() override;

protected slots:
	void timerTimeout() override;

protected:
	void stop() override;

	robotModel::PortInfo mPort;

private:
	/// Shall be reimplemented to do value check when new data from sensor is ready.
	virtual void responseSlot(int reading) = 0;
};

}
}
}
