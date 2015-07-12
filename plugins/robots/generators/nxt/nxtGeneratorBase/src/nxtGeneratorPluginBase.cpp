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

#include "nxtGeneratorBase/nxtGeneratorPluginBase.h"

#include <nxtKit/blocks/nxtBlocksFactory.h>

#include "src/robotModel/nxtGeneratorRobotModel.h"

using namespace nxt;

NxtGeneratorPluginBase::NxtGeneratorPluginBase(const QString &robotName, const QString &robotFriendlyName
		, int priority)
	: mRobotModel(new robotModel::NxtGeneratorRobotModel(kitId()
			, "nxtGeneratorRobot", robotName, robotFriendlyName, priority))
	, mBlocksFactory(new blocks::NxtBlocksFactory)
{
}

NxtGeneratorPluginBase::~NxtGeneratorPluginBase()
{
}

QString NxtGeneratorPluginBase::kitId() const
{
	return "nxtKit";
}

QList<kitBase::robotModel::RobotModelInterface *> NxtGeneratorPluginBase::robotModels()
{
	return { mRobotModel.data() };
}

kitBase::blocksBase::BlocksFactoryInterface *NxtGeneratorPluginBase::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return mBlocksFactory;
}

QList<kitBase::AdditionalPreferences *> NxtGeneratorPluginBase::settingsWidgets()
{
	return {};
}
