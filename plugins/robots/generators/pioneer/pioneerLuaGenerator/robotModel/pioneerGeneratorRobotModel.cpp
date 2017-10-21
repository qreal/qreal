/* Copyright 2017 QReal Research Group
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

#include "pioneerGeneratorRobotModel.h"

using namespace pioneer::lua;

PioneerGeneratorRobotModel::PioneerGeneratorRobotModel(const QString &kitId
		, const QString &robotId
		, const QString &name
		, const QString &friendlyName
		, int priority)
	: kitBase::robotModel::CommonRobotModel(kitId, robotId)
	, mName(name)
	, mFriendlyName(friendlyName)
	, mPriority(priority)
{
}

QString PioneerGeneratorRobotModel::name() const
{
	return mName;
}

QString PioneerGeneratorRobotModel::friendlyName() const
{
	return mFriendlyName;
}

bool PioneerGeneratorRobotModel::needsConnection() const
{
	return false;
}

bool PioneerGeneratorRobotModel::interpretedModel() const
{
	return false;
}

int PioneerGeneratorRobotModel::priority() const
{
	return mPriority;
}
