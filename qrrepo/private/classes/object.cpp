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

#include "object.h"

#include <QtCore/QDebug>

#include <qrkernel/exception/exception.h>
#include <qrkernel/ids.h>

#include "logicalObject.h"
#include "graphicalObject.h"
#include "qrrepo/private/valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

Object::Object(const Id &id)
	: mId(id)
{
}

Object::Object(const QDomElement &element)
	: mId(Id::loadFromString(element.attribute("id", "")))
{
	if (mId.isNull()) {
		throw Exception("Id deserialization failed");
	}

	mParent = ValuesSerializer::deserializeId(element.attribute("parent", ""));

	foreach (const Id &child, ValuesSerializer::deserializeIdList(element, "children")) {
		mChildren.append(child);
	}

	QDomNodeList propertiesList = element.elementsByTagName("properties");
	if (propertiesList.count() != 1) {
		throw Exception("Incorrect element: children list must appear once");
	}

	QDomElement properties = propertiesList.at(0).toElement();
	ValuesSerializer::deserializeNamedVariantsMap(mProperties, properties);
}

Object::~Object()
{
}

void Object::replaceProperties(const QString &value, const QString &newValue)
{
	foreach (const QVariant &val, mProperties.values()) {
		if (val.toString().contains(value)) {
			mProperties[mProperties.key(val)] = newValue;
		}
	}
}

Object *Object::clone(QHash<Id, Object*> &objHash) const
{
	Object * const result = createClone();
	objHash.insert(result->id(), result);

	result->mParent = mParent;

	foreach (const Id &childId, mChildren) {
		Object * const child = objHash[childId]->clone(objHash);
		child->setParent(mId);
		result->addChild(child->id());
	}

	// Using copy constructor.
	result->mProperties = mProperties;

	return result;
}

void Object::setParent(const Id &parent)
{
	mParent = parent;
}

void Object::addChild(const Id &child)
{
	if (mChildren.contains(child)) {
		throw Exception("Object " + mId.toString() + ": adding existing child " + child.toString());
	}

	mChildren.append(child);
}

void Object::removeChild(const Id &child)
{
	if (mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Object " + mId.toString() + ": removing nonexistent child " + child.toString());
	}
}

void Object::copyPropertiesFrom(const Object &src)
{
	mProperties = src.mProperties;
}

IdList Object::children() const
{
	return mChildren;
}

void Object::stackBefore(const qReal::Id &element, const qReal::Id &sibling)
{
	if (element == sibling) {
		return;
	}

	if (!mChildren.contains(element)) {
		throw Exception("Object " + mId.toString() + ": moving nonexistent child " + element.toString());
	}

	if (!mChildren.contains(sibling)) {
		throw Exception("Object " + mId.toString() + ": stacking before nonexistent child " + sibling.toString());
	}

	mChildren.removeOne(element);
	mChildren.insert(mChildren.indexOf(sibling), element);
}

Id Object::parent() const
{
	return mParent;
}

void Object::setProperty(const QString &name, const QVariant &value)
{
	if (value == QVariant()) {
		qDebug() << "Empty QVariant set as a property for " << id().toString();
		qDebug() << ", property name " << name;
		Q_ASSERT(!"Empty QVariant set as a property");
	}

	mProperties.insert(name,value);
}

void Object::setProperties(QMap<QString, QVariant> const &properties)
{
	mProperties = properties;
}

QVariant Object::property(const QString &name) const
{
	if (mProperties.contains(name)) {
		return mProperties[name];
	} else if (name == "backReferences") {
		return QVariant();
	} else {
		// TODO: replace with exception when there is some kind of model migration tool
		return QVariant();
	}
}

void Object::setBackReference(const qReal::Id &reference)
{
	IdList references = mProperties["backReferences"].value<IdList>();
	references << reference;
	mProperties.insert("backReferences", qReal::IdListHelper::toVariant(references));
}

void Object::removeBackReference(const qReal::Id &reference)
{
	if (!mProperties.contains("backReferences")) {
		throw Exception("Object " + mId.toString() + ": removing nonexsistent reference " + reference.toString());
	}

	IdList references = mProperties["backReferences"].value<IdList>();
	if (!references.contains(reference)) {
		throw Exception("Object " + mId.toString() + ": removing nonexsistent reference " + reference.toString());
	}

	references.removeOne(reference);
	mProperties.insert("backReferences", qReal::IdListHelper::toVariant(references));
}

void Object::setTemporaryRemovedLinks(const QString &direction, const qReal::IdList &listValue)
{
	mTemporaryRemovedLinks.insert(direction, listValue);
}

IdList Object::temporaryRemovedLinksAt(const QString &direction) const
{
	return mTemporaryRemovedLinks.value(direction);
}

IdList Object::temporaryRemovedLinks() const
{
	return temporaryRemovedLinksAt("to") << temporaryRemovedLinksAt("from") << temporaryRemovedLinksAt(QString());
}

void Object::removeTemporaryRemovedLinksAt(const QString &direction)
{
	if (mTemporaryRemovedLinks.contains(direction)) {
		mProperties.remove(direction);
	}
}

void Object::removeTemporaryRemovedLinks()
{
	removeTemporaryRemovedLinksAt("from");
	removeTemporaryRemovedLinksAt("to");
	removeTemporaryRemovedLinksAt(QString());
}

bool Object::hasProperty(const QString &name, bool sensitivity, bool regExpression) const
{
	QStringList properties = mProperties.keys();
	Qt::CaseSensitivity caseSensitivity;

	if (sensitivity) {
		caseSensitivity = Qt::CaseSensitive;
	} else {
		caseSensitivity = Qt::CaseInsensitive;
	}

	if (regExpression) {
		QRegExp regExp(name, caseSensitivity);
		return !properties.filter(regExp).isEmpty();
	} else {
		return properties.contains(name, caseSensitivity);
	}
}

void Object::removeProperty(const QString &name)
{
	if (mProperties.contains(name)) {
		mProperties.remove(name);
	} else {
		throw Exception("Object " + mId.toString() + ": removing nonexistent property " + name);
	}
}

Id Object::id() const
{
	return mId;
}

QMapIterator<QString, QVariant> Object::propertiesIterator() const
{
	return QMapIterator<QString, QVariant>(mProperties);
}

QMap<QString, QVariant> Object::properties() const
{
	return mProperties;
}

QDomElement Object::serialize(QDomDocument &document) const
{
	QDomElement result = document.createElement("object");
	result.setAttribute("id", id().toString());
	result.setAttribute("parent", parent().toString());
	result.appendChild(ValuesSerializer::serializeIdList("children", children(), document));
	result.appendChild(ValuesSerializer::serializeNamedVariantsMap("properties", mProperties, document));
	return result;
}
