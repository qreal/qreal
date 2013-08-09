#pragma once

#include "../../../qrkernel/ids.h"

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
	explicit Object(qReal::Id const &id);

	/// Deserializing constructor.
	/// @param element - root of XML DOM subtree with serialized object.
	explicit Object(QDomElement const &element);

	virtual ~Object();

	/// Replacing property values that contains input value with new value.
	/// @param value - input value that should be contained by any property of element.
	/// @param newValue - string representation of value with what property values should be replaced.
	void replaceProperties(QString const value, QString const &newValue);

	/// Creates deep copy of this object also creating new objects for children (parent remains as is).
	/// @param objHash - hashtable with cloned object and its children, will be used to get information
	///        about children and to add clone (and clones of all children).
	Object *clone(QHash<qReal::Id, Object *> &objHash) const;

	/// Serializes contents of an object to XML DOM subtree.
	/// @param document - document to which will belong created subtree.
	virtual QDomElement serialize(QDomDocument &document) const;

	void setParent(qReal::Id const &parent);
	void addChild(qReal::Id const &child);
	void removeChild(qReal::Id const &child);
	qReal::IdList children() const;
	qReal::Id parent() const;
	QVariant property(QString const &name) const;
	bool hasProperty(QString const &name, bool sensitivity = false, bool regExpression = false) const;
	void setProperty(QString const &name, QVariant const &value);
	void removeProperty(QString const &name);
	void setBackReference(qReal::Id const &reference);
	void removeBackReference(qReal::Id const &reference);

	void setProperties(QMap<QString, QVariant> const &properties);
	void copyPropertiesFrom(Object const &src);
	// TODO: omg
	QMap<QString, QVariant> properties() const;
	QMapIterator<QString, QVariant> propertiesIterator() const;

	qReal::Id id() const;

	void setTemporaryRemovedLinks(QString const &direction, qReal::IdList const &listValue);
	qReal::IdList temporaryRemovedLinksAt(QString const &direction) const;
	qReal::IdList temporaryRemovedLinks() const;
	void removeTemporaryRemovedLinksAt(QString const &direction);
	void removeTemporaryRemovedLinks();

	/// Stacks item element before sibling (they should have the same parent).
	void stackBefore(qReal::Id const &element, qReal::Id const &sibling);

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
