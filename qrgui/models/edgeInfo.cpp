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

#include "edgeInfo.h"

using namespace qReal;

EdgeInfo::EdgeInfo()
	: ElementInfo(true)
{
}

QDataStream &EdgeInfo::serialize(QDataStream &out) const
{
	return ElementInfo::serialize(out) << srcId << dstId << portFrom << portTo << configuration << shapeType;
}

QDataStream &EdgeInfo::deserialize(QDataStream &in)
{
	return ElementInfo::deserialize(in) >> srcId >> dstId >> portFrom >> portTo >> configuration >> shapeType;
}

bool EdgeInfo::equals(const ElementInfo &other) const
{
	const EdgeInfo *otherEdge = dynamic_cast<const EdgeInfo *>(&other);
	if (!otherEdge) {
		return false;
	}

	return ElementInfo::equals(other)
			&& srcId == otherEdge->srcId
			&& dstId == otherEdge->dstId
			&& portFrom == otherEdge->portFrom
			&& portTo == otherEdge->portTo
			&& configuration == otherEdge->configuration;
}

QDataStream &operator<< (QDataStream &out, const EdgeInfo &data)
{
	return data.serialize(out);
}

QDataStream &operator>> (QDataStream &in, EdgeInfo &data)
{
	return data.deserialize(in);
}

bool operator== (const EdgeInfo &first, const EdgeInfo &second)
{
	return first.equals(second);
}

ElementInfo EdgeInfo::convertToSimpleInfo() const
{
	ElementInfo element(*this);
	element.graphicalProperties["configuration"] = configuration;
	element.graphicalProperties["linkShape"] = shapeType;

	element.graphicalProperties["from"] = srcId.toVariant();
	element.graphicalProperties["to"] = dstId.toVariant();

	element.graphicalProperties["fromPort"] = portFrom;
	element.graphicalProperties["toPort"] = portTo;

	return element;
}
