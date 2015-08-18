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

#include "kitBase/blocksBase/common/waitBlock.h"

#include "kitBase/robotModel/robotModelUtils.h"
#include "utils/timelineInterface.h"
#include "utils/abstractTimer.h"

using namespace kitBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitBlock::WaitBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	mActiveWaitingTimer.reset(robotModel.timeline().produceTimer());
	connect(mActiveWaitingTimer.data(), &utils::AbstractTimer::timeout, this, &WaitBlock::timerTimeout);
	mActiveWaitingTimer->setInterval(20);
	mActiveWaitingTimer->setRepeatable(true);
}

WaitBlock::~WaitBlock()
{
}

void WaitBlock::setFailedStatus()
{
	Block::setFailedStatus();
	stopActiveTimerInBlock();
}

void WaitBlock::processResponce(int reading, int targetValue)
{
	const QString sign = stringProperty("Sign");
	if (sign == "equals" && reading == targetValue) {
		stop();
	} else if (sign == "greater" && reading > targetValue) {
		stop();
	} else if (sign == "less" && reading < targetValue) {
		stop();
	} else if (sign == "notLess" && reading >= targetValue) {
		stop();
	} else if (sign == "notGreater" && reading <= targetValue) {
		stop();
	}
}

void WaitBlock::stop()
{
	mActiveWaitingTimer->stop();
	// Emitting done() immediately will switch current block right during SensorVariablesUpdater
	// doing his job. This may cause bad side effects.
	QTimer::singleShot(0, [this]() { emit done(mNextBlockId); });
}

void WaitBlock::failureSlot()
{
	mActiveWaitingTimer->stop();
	emit failure();
}

void WaitBlock::stopActiveTimerInBlock()
{
	mActiveWaitingTimer->stop();
}

QMap<PortInfo, DeviceInfo> WaitBlock::usedDevices()
{
	const DeviceInfo deviceInfo = device();
	const PortInfo portInfo = RobotModelUtils::findPort(mRobotModel, port(), deviceInfo.direction());
	QMap<PortInfo, DeviceInfo> result;
	if (!deviceInfo.isNull() && portInfo.isValid()) {
		result[portInfo] = deviceInfo;
	}

	return result;
}

QString WaitBlock::port()
{
	return eval<QString>("Port");
}
