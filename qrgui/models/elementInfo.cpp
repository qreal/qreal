/* Copyright 2015 Dmitry Mordvinov
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

#include "elementInfo.h"

using namespace qReal;

ElementInfo::ElementInfo(const Id &id
		, const Id &logicalId
		, const Id &parent
		, const QString &name
		, const QPointF &position)
	: mId(id)
	, mLogicalId(logicalId)
	, mParent(parent)
	, mName(name)
	, mPosition(position)
{
}

Id ElementInfo::id() const
{
	return mId;
}

Id ElementInfo::logicalId() const
{
	return mLogicalId;
}

Id ElementInfo::parent() const
{
	return mParent;
}

QString ElementInfo::name() const
{
	return mName;
}

QPointF ElementInfo::position() const
{
	return mPosition;
}

ElementInfo &ElementInfo::withLogicalId(const Id &logicalId)
{
	mLogicalId = logicalId;
	return *this;
}
