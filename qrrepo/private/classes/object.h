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

#pragma once

#include <qrkernel/ids.h>

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

namespace qrRepo {
namespace details {

/// Abstract class, general object in repository. Has id, parent, children and properties, able to
/// serialize/deserialize and clone itself.
class Object
{
public:
	/// Constructor.
	/// @param id - id of a new object.
	explicit Object(const qReal::Id &id);

	/// Deserializing constructor.
	/// @param element - root of XML DOM subtree with serialized object.
	explicit Object(const QDomElement &element);

	virtual ~Object();

	/// Replacing property values that contains input value with new value.
	/// @param value - input value that should be contained by any property of element.
	/// @param newValue - string representation of value with what property values should be replaced.
	void replaceProperties(const QString &value, const QString &newValue);

	/// Creates deep copy of this object also creating new objects for children (parent remains as is).
	/// @param objHash - hashtable with cloned object and its children, will be used to get information
	///        about children and to add clone (and clones of all children).
	Object *clone(QHash<qReal::Id, Object *> &objHash) const;

	/// Serializes contents of an object to XML DOM subtree.
	/// @param document - document to which will belong created subtree.
	virtual QDomElement serialize(QDomDocument &document) const;

	void setParent(const qReal::Id &parent);
	void addChild(const qReal::Id &child);
	void removeChild(const qReal::Id &child);
	qReal::IdList children() const;
	qReal::Id parent() const;
	QVariant property(const QString &name) const;
	bool hasProperty(const QString &name, bool sensitivity = false, bool regExpression = false) const;
	void setProperty(const QString &name, const QVariant &value);
	void removeProperty(const QString &name);
	void setBackReference(const qReal::Id &reference);
	void removeBackReference(const qReal::Id &reference);

	void setProperties(QMap<QString, QVariant> const &properties);
	void copyPropertiesFrom(const Object &src);
	// TODO: omg
	QMap<QString, QVariant> properties() const;
	QMapIterator<QString, QVariant> propertiesIterator() const;

	qReal::Id id() const;

	void setTemporaryRemovedLinks(const QString &direction, const qReal::IdList &listValue);
	qReal::IdList temporaryRemovedLinksAt(const QString &direction) const;
	qReal::IdList temporaryRemovedLinks() const;
	void removeTemporaryRemovedLinksAt(const QString &direction);
	void removeTemporaryRemovedLinks();

	/// Stacks item element before sibling (they should have the same parent).
	void stackBefore(const qReal::Id &element, const qReal::Id &sibling);

	/// Returns true, if it is logical object, false, if graphical.
	virtual bool isLogicalObject() const = 0;

protected:
	/// Implemented in derived classes to create a clone and init it with specific fields.
	virtual Object *createClone() const = 0;

	const qReal::Id mId;
	qReal::Id mParent;
	qReal::IdList mChildren;
	QMap<QString, QVariant> mProperties;
	QMap<QString, qReal::IdList> mTemporaryRemovedLinks;
};

}

}
