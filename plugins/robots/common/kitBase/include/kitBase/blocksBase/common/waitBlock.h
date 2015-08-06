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

#include <QtCore/QScopedPointer>

#include <kitBase/blocksBase/robotsBlock.h>

namespace utils {
class AbstractTimer;
}

namespace kitBase {
namespace blocksBase {
namespace common {

/// A base for all blocks that wait for sensor or button or something other.
class ROBOTS_KIT_BASE_EXPORT WaitBlock : public RobotsBlock
{
	Q_OBJECT

public:
	/// Constructor, takes current robot model as parameter.
	explicit WaitBlock(robotModel::RobotModelInterface &robotModel);
	~WaitBlock() override;

	void setFailedStatus() override;

	QMap<robotModel::PortInfo, robotModel::DeviceInfo> usedDevices() override;

protected slots:
	/// Shall be called when something goes wrong and there will be no response from real device.
	void failureSlot();

protected:
	/// Stops waiting timer.
	/// @todo Shall not be virtual.
	virtual void stopActiveTimerInBlock();

	/// Checks that current sensor reading greater, less or so on than target value and stops waiting if reading is ok.
	void processResponce(int reading, int targetValue);

	/// Stops waiting and transfers control to a next block.
	virtual void stop();

	/// Implementation must return the port of the the device used in the block.
	/// Default implementation works with the port specified in the 'Port' property of the block.
	virtual QString port();

	/// Implementation must return the type of the waited device or DeviceInfo() if no such one.
	virtual robotModel::DeviceInfo device() const = 0;

	/// @todo Make it private.
	robotModel::RobotModelInterface &mRobotModel;
	QScopedPointer<utils::AbstractTimer> mActiveWaitingTimer;

private slots:
	/// Shall be redefined in subclasses to request for a sensor or button to read its value.
	virtual void timerTimeout() = 0;
};

}
}
}
