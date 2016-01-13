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

#include "elementInfo.h"

#include <QtCore/QMimeData>

#include <qrkernel/definitions.h>

using namespace qReal;

ElementInfo::ElementInfo()
	: mIsEdge(false)
{
}

ElementInfo::ElementInfo(bool isEdge)
	: mIsEdge(isEdge)
{
}

ElementInfo::ElementInfo(const Id &id
		, const Id &logicalId
		, const QString &name
		, const Id &explosionTarget
		, bool isEdge)
	: id(id)
	, logicalId(logicalId)
	, logicalProperties({{"name", name}})
	, explosionTarget(explosionTarget)
	, mIsEdge(isEdge)
{
}

ElementInfo::ElementInfo(const Id &id
		, const Id &logicalId
		, const Id &logicalParent
		, const Id &graphicalParent
		, const QMap<QString, QVariant> &logicalProperties
		, const QMap<QString, QVariant> &graphicalProperties
		, const Id &explosionTarget
		, bool isEdge)
	: id(id)
	, logicalId(logicalId)
	, logicalParent(logicalParent)
	, graphicalParent(graphicalParent)
	, logicalProperties(logicalProperties)
	, graphicalProperties(graphicalProperties)
	, explosionTarget(explosionTarget)
	, mIsEdge(isEdge)
{
}

ElementInfo::~ElementInfo()
{
}

QDataStream &ElementInfo::serialize(QDataStream &out) const
{
	return out << id << logicalId << logicalParent << graphicalParent
			<< logicalProperties << graphicalProperties << explosionTarget << mIsEdge;
}

QDataStream &ElementInfo::deserialize(QDataStream &in)
{
	return in >> id >> logicalId >> logicalParent >> graphicalParent
			>> logicalProperties >> graphicalProperties >> explosionTarget >> mIsEdge;
}

bool ElementInfo::equals(const ElementInfo &other) const
{
	return id == other.id
			&& logicalId == other.logicalId
			&& logicalParent == other.logicalParent
			&& graphicalParent == other.graphicalParent
			&& logicalProperties == other.logicalProperties
			&& graphicalProperties == other.graphicalProperties
			&& explosionTarget == other.explosionTarget
			&& mIsEdge == other.mIsEdge;
}

QMimeData *ElementInfo::mimeData() const
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);
	stream << *this;
	QMimeData * const mimeData = new QMimeData;
	mimeData->setData(DEFAULT_MIME_TYPE, data);
	return mimeData;
}

ElementInfo ElementInfo::fromMimeData(const QMimeData *mimeData)
{
	QByteArray data = mimeData->data(DEFAULT_MIME_TYPE);
	QDataStream inStream(&data, QIODevice::ReadOnly);

	ElementInfo result;
	inStream >> result;
	return result;
}

bool ElementInfo::isEdge() const
{
	return mIsEdge;
}

Id ElementInfo::parent() const
{
	return logicalId == id ? logicalParent : graphicalParent;
}

QString ElementInfo::name() const
{
	return logicalProperties["name"].toString();
}

QPointF ElementInfo::position() const
{
	return graphicalProperties["position"].toPointF();
}

Id ElementInfo::newId()
{
	if (id.isNull() && id == Id::rootId()) {
		return id;
	}

	return (id = id.sameTypeId());
}

Id ElementInfo::newLogicalId()
{
	if (id.isNull() && id == Id::rootId()) {
		return logicalId;
	}

	return (logicalId = id.sameTypeId());
}

void ElementInfo::setPos(const QPointF &position)
{
	graphicalProperties["position"] = position;
}

QDataStream &operator<< (QDataStream &out, const ElementInfo &data)
{
	return data.serialize(out);
}

QDataStream &operator>> (QDataStream &in, ElementInfo &data)
{
	return data.deserialize(in);
}

bool operator==(const ElementInfo &first, const ElementInfo &second)
{
	return first.equals(second);
}
