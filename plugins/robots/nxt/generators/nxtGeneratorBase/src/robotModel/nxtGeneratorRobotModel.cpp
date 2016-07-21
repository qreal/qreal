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

#include "nxtGeneratorRobotModel.h"

using namespace nxt::robotModel;

NxtGeneratorRobotModel::NxtGeneratorRobotModel(const QString &kitId
		, const QString &robotId
		, const QString &name
		, const QString &friendlyName
		, int priority)
	: NxtRobotModelBase(kitId, robotId)
	, mName(name)
	, mFriendlyName(friendlyName)
	, mPriority(priority)
{
}

QString NxtGeneratorRobotModel::name() const
{
	return mName;
}

QString NxtGeneratorRobotModel::friendlyName() const
{
	return mFriendlyName;
}

bool NxtGeneratorRobotModel::needsConnection() const
{
	return false;
}

bool NxtGeneratorRobotModel::interpretedModel() const
{
	return false;
}

int NxtGeneratorRobotModel::priority() const
{
	return mPriority;
}
