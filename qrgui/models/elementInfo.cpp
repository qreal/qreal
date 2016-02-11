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
	: mId(id)
	, mLogicalId(logicalId)
	, mLogicalProperties({{"name", name}})
	, mExplosionTarget(explosionTarget)
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
	: mId(id)
	, mLogicalId(logicalId)
	, mLogicalParent(logicalParent)
	, mGraphicalParent(graphicalParent)
	, mLogicalProperties(logicalProperties)
	, mGraphicalProperties(graphicalProperties)
	, mExplosionTarget(explosionTarget)
	, mIsEdge(isEdge)
{
}

ElementInfo::~ElementInfo()
{
}

QDataStream &ElementInfo::serialize(QDataStream &out) const
{
	return out << mId << mLogicalId << mLogicalParent << mGraphicalParent
			<< mLogicalProperties << mGraphicalProperties << mExplosionTarget << mIsEdge;
}

QDataStream &ElementInfo::deserialize(QDataStream &in)
{
	return in >> mId >> mLogicalId >> mLogicalParent >> mGraphicalParent
			>> mLogicalProperties >> mGraphicalProperties >> mExplosionTarget >> mIsEdge;
}

bool ElementInfo::equals(const ElementInfo &other) const
{
	return mId == other.mId
			&& mLogicalId == other.mLogicalId
			&& mLogicalParent == other.mLogicalParent
			&& mGraphicalParent == other.mGraphicalParent
			&& mLogicalProperties == other.mLogicalProperties
			&& mGraphicalProperties == other.mGraphicalProperties
			&& mExplosionTarget == other.mExplosionTarget
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
	return mLogicalId == mId ? mLogicalParent : mGraphicalParent;
}

QString ElementInfo::name() const
{
	return mLogicalProperties["name"].toString();
}

QPointF ElementInfo::position() const
{
	return mGraphicalProperties["position"].toPointF();
}

Id ElementInfo::newId()
{
	if (mId.isNull() && mId == Id::rootId()) {
		return mId;
	}

	return (mId = mId.sameTypeId());
}

Id ElementInfo::newLogicalId()
{
	if (mId.isNull() && mId == Id::rootId()) {
		return mLogicalId;
	}

	return (mLogicalId = mId.sameTypeId());
}

void ElementInfo::setPos(const QPointF &position)
{
	mGraphicalProperties["position"] = position;
}

void ElementInfo::setGraphicalProperty(const QString &propertyName, const QVariant &propertyValue)
{
	mGraphicalProperties[propertyName] = propertyValue;
}

const Id &ElementInfo::explosionTarget() const
{
	return mExplosionTarget;
}

const Id &ElementInfo::id() const
{
	return mId;
}

const Id &ElementInfo::logicalId() const
{
	return mLogicalId;
}

const Id &ElementInfo::logicalParent() const
{
	return mLogicalParent;
}

void ElementInfo::setLogicalId(const Id &id)
{
	mLogicalId = id;
}

void ElementInfo::setLogicalParent(const Id &parent)
{
	mLogicalParent = parent;
}

const Id &ElementInfo::graphicalParent() const
{
	return mGraphicalParent;
}

void ElementInfo::setGraphicalParent(const Id &parent)
{
	mGraphicalParent = parent;
}

const QList<QString> ElementInfo::logicalProperties() const
{
	return mLogicalProperties.keys();
}

QVariant ElementInfo::logicalProperty(const QString &propertyName) const
{
	return mLogicalProperties[propertyName];
}

void ElementInfo::setLogicalProperty(const QString &propertyName, const QVariant &propertyValue)
{
	mLogicalProperties[propertyName] = propertyValue;
}

void ElementInfo::setAllLogicalProperties(const QMap<QString, QVariant> &logicalProperties)
{
	mLogicalProperties = logicalProperties;
}

const QList<QString> ElementInfo::graphicalProperties() const
{
	return mGraphicalProperties.keys();
}

QVariant ElementInfo::graphicalProperty(const QString &propertyName) const
{
	return mGraphicalProperties[propertyName];
}

void ElementInfo::setAllGraphicalProperties(const QMap<QString, QVariant> &graphicalProperties)
{
	mGraphicalProperties = graphicalProperties;
}

QDataStream &qReal::operator<< (QDataStream &out, const ElementInfo &data)
{
	return data.serialize(out);
}

QDataStream &qReal::operator>> (QDataStream &in, ElementInfo &data)
{
	return data.deserialize(in);
}

bool qReal::operator==(const ElementInfo &first, const ElementInfo &second)
{
	return first.equals(second);
}
