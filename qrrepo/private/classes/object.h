#pragma once

#include "../../../qrkernel/ids.h"

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

namespace qrRepo {
namespace details {

class Object
{
public:
	explicit Object(qReal::Id const &id);
	explicit Object(QDomElement const &element);
	virtual ~Object();

	/// Replacing property values that contains input value with new value.
	/// @param value - input value that should be contained by any property of element.
	/// @param newValue - string representation of value with what property values should be replaced.
	void replaceProperties(QString const value, QString newValue);

	Object *clone(QHash<qReal::Id, Object*> &objHash) const;
	Object *clone(qReal::Id const &parent, QHash<qReal::Id, Object*> &objHash) const;

	virtual QDomElement serialize(QDomDocument &document) const;

	void setParent(const qReal::Id &parent);
	void removeParent();
	void addChild(const qReal::Id &child);
	void removeChild(const qReal::Id &child);
	void copyPropertiesFrom(const Object &src);
	qReal::IdList children() const;
	qReal::Id parent() const;
	QVariant property(QString const &name) const;
	bool hasProperty(QString const &name, bool sensitivity = false, bool regExpression = false) const;
	void setProperty(QString const &name, const QVariant &value);
	void setProperties(QMap<QString, QVariant> const &properties);
	void setBackReference(qReal::Id const &reference);
	void removeBackReference(qReal::Id const &reference);
	void removeProperty(QString const &name);
	QMap<QString, QVariant> properties();
	qReal::Id id() const;
	QMapIterator<QString, QVariant> propertiesIterator() const;
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
	virtual Object *createClone() const = 0;

	const qReal::Id mId;
	qReal::Id mParent;
	qReal::IdList mChildren;
	QMap<QString, QVariant> mProperties;
	QMap<QString, qReal::IdList> mTemporaryRemovedLinks;
};

}

}
