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

#include "serializationData.h"

QDataStream &operator<< (QDataStream &out, const NodeData &data)
{
	out << data.id << data.logicalId << data.parentId << data.pos << data.contents
			<< data.graphicalProperties << data.logicalProperties << data.explosion;
	return out;
}

QDataStream &operator>> (QDataStream &in, NodeData &data)
{
	in >> data.id >> data.logicalId >> data.parentId >> data.pos >> data.contents
			>> data.graphicalProperties >> data.logicalProperties >> data.explosion;
	return in;
}

QDataStream &operator<< (QDataStream &out, const EdgeData &data)
{
	out << data.id << data.logicalId << data.srcId << data.dstId
			<< data.portFrom << data.portTo
			<< data.configuration << data.pos
			<< data.shapeType << data.logicalProperties;
	return out;
}

QDataStream &operator>> (QDataStream &in, EdgeData &data)
{
	in >> data.id >> data.logicalId >> data.srcId >> data.dstId
			>> data.portFrom >> data.portTo
			>> data.configuration >> data.pos
			>> data.shapeType >> data.logicalProperties;
	return in;
}

bool operator== (const NodeData &first, const NodeData &second)
{
	return first.id == second.id
			&& first.parentId == second.parentId
			&& first.pos == second.pos
			&& first.logicalProperties == second.logicalProperties
			&& first.graphicalProperties == second.graphicalProperties
			&& first.contents == second.contents
			&& first.logicalId == second.logicalId
			&& first.explosion == second.explosion;
}

bool operator== (const EdgeData &first, const EdgeData &second)
{
	return first.id == second.id && first.logicalId == second.logicalId
			&& first.srcId == second.srcId && first.dstId == second.dstId
			&& first.portFrom == second.portFrom && first.portTo == second.portTo
			&& first.configuration == second.configuration && first.pos == second.pos
			&& first.logicalProperties == second.logicalProperties;
}
