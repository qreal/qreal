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

#include <kitBase/blocksBase/common/waitForButtonBlock.h>

#include <utils/abstractTimer.h>
#include <kitBase/robotModel/robotModelInterface.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace kitBase::blocksBase::common;
using namespace kitBase::robotModel;

WaitForButtonBlock::WaitForButtonBlock(robotModel::RobotModelInterface &robotModel)
	: WaitBlock(robotModel)
{
}

void WaitForButtonBlock::run()
{
	const QString port = eval<QString>("Button");

	mButton = RobotModelUtils::findDevice<robotParts::Button>(mRobotModel, port);

	if (!mButton) {
		mActiveWaitingTimer->stop();
		error(tr("Incorrect button port %1").arg(port));
		return;
	}

	connect(mButton, &robotModel::robotParts::Button::newData
			, this, &WaitForButtonBlock::responseSlot, Qt::UniqueConnection);

	mButton->read();
	mActiveWaitingTimer->start();
}

void WaitForButtonBlock::timerTimeout()
{
	mButton->read();
}

void WaitForButtonBlock::responseSlot(int isPressed)
{
	if (isPressed) {
		stop();
	}
}

DeviceInfo WaitForButtonBlock::device() const
{
	return DeviceInfo::create<robotModel::robotParts::Button>();
}
