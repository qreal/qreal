/* Copyright 2015-2016 Dmitry Mordvinov
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

#include "nodeInfo.h"

using namespace qReal;

NodeInfo::NodeInfo()
{
}

NodeInfo::NodeInfo(const Id &id
		, const Id &logicalId
		, const Id &logicalParent
		, const Id &graphicalParent
		, const QMap<QString, QVariant> &logicalProperties
		, const QMap<QString, QVariant> &graphicalProperties
		, const Id &explosionTarget)
	: ElementInfo(id, logicalId, graphicalParent, logicalParent
			, logicalProperties, graphicalProperties, explosionTarget, false)
{
}

QDataStream &operator<< (QDataStream &out, const NodeInfo &data)
{
	return data.serialize(out);
}

QDataStream &operator>> (QDataStream &in, NodeInfo &data)
{
	return data.deserialize(in);
}

bool operator== (const NodeInfo &first, const NodeInfo &second)
{
	return first.equals(second);
}
