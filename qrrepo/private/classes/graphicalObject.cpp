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

#include "graphicalObject.h"

#include <qrkernel/exception/exception.h>

#include "qrrepo/private/valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

GraphicalObject::GraphicalObject(const qReal::Id &id, const Id &parent, const qReal::Id &logicalId)
	: Object(id)
	, mLogicalId(logicalId)
{
	mParent = parent;
}

GraphicalObject::GraphicalObject(const QDomElement &element)
	: Object(element)
{
	mLogicalId = Id::loadFromString(element.attribute("logicalId"));
	if (mLogicalId.isNull()) {
		throw Exception("Logical id not found for graphical object");
	}

	const QDomElement graphicalParts = element.firstChildElement("graphicalParts");
	if (!graphicalParts.isNull()) {
		for (QDomElement part = graphicalParts.firstChildElement(); !part.isNull(); part = part.nextSiblingElement()) {
			GraphicalPart * const deserializedPart = new GraphicalPart(part);

			const QString indexString = part.attribute("index");
			if (indexString.isEmpty()) {
				throw Exception("No \"index\" attribute in graphical part");
			}

			mGraphicalParts.insert(indexString.toInt(), deserializedPart);
		}
	}
}

GraphicalObject::~GraphicalObject()
{
	qDeleteAll(mGraphicalParts.values());
}

QDomElement GraphicalObject::serialize(QDomDocument &document) const
{
	QDomElement result = Object::serialize(document);
	result.setAttribute("logicalId", mLogicalId.toString());

	QDomElement graphicalParts = document.createElement("graphicalParts");
	result.appendChild(graphicalParts);

	for (QHash<int, GraphicalPart *>::const_iterator i = mGraphicalParts.constBegin();
			i != mGraphicalParts.constEnd();
			++i)
	{
		const QDomElement graphicalPart = i.value()->serialize(i.key(), document);
		graphicalParts.appendChild(graphicalPart);
	}

	return result;
}

void GraphicalObject::createGraphicalPart(int index)
{
	if (mGraphicalParts.contains(index)) {
		throw Exception("Part with that index already exists");
	}

	GraphicalPart * const part = new GraphicalPart();
	mGraphicalParts.insert(index, part);
}

QList<int> GraphicalObject::graphicalParts() const
{
	return mGraphicalParts.keys();
}

QVariant GraphicalObject::graphicalPartProperty(int index, const QString &name) const
{
	if (!mGraphicalParts.contains(index)) {
		throw Exception("Tryng to get property of non-existing graphical part");
	}

	return mGraphicalParts[index]->property(name);
}

void GraphicalObject::setGraphicalPartProperty(int index, const QString &name, const QVariant &value)
{
	if (!mGraphicalParts.contains(index)) {
		throw Exception("Tryng to set property of non-existing graphical part");
	}

	mGraphicalParts[index]->setProperty(name, value);
}

Object *GraphicalObject::createClone() const
{
	GraphicalObject * const clone = new GraphicalObject(mId.sameTypeId(), mParent, mLogicalId);

	for (QHash<int, GraphicalPart *>::const_iterator i = mGraphicalParts.constBegin();
			i != mGraphicalParts.constEnd();
			++i)
	{
		GraphicalPart * const partClone = i.value()->clone();
		clone->mGraphicalParts.insert(i.key(), partClone);
	}

	return clone;
}

Id GraphicalObject::logicalId() const
{
	return mLogicalId;
}

bool GraphicalObject::isLogicalObject() const
{
	return false;
}
