#include "object.h"
#include "../../../qrkernel/exception/exception.h"

#include <QDebug>

using namespace qrRepo::details;
using namespace qReal;

Object::Object(const Id &id, const Id &parent)
	: mId(id)
{
	setParent(parent);
}

void Object::replaceProperties(QString const value, QString newValue)
{
	foreach (QVariant val, mProperties.values()) {
		if (val.toString().contains(value)) {
			mProperties[mProperties.key(val)] = newValue;
		}
	}
}

Object::Object(const Id &id, const Id &parent, const qReal::Id &logicalId)
	: mId(id), mLogicalId(logicalId)
{
	setParent(parent);
}

Object::Object(const Id &id) : mId(id)
{
}

Object *Object::clone(QHash<Id, Object*> &objHash) const
{
	Id resultId = id().sameTypeId();
	Object *result = new Object(resultId);
	objHash.insert(resultId, result);

	foreach (Id childId, mChildren) {
		Object *child = objHash[childId]->clone(id(), objHash);
		result->addChild(child->id());
	}

	//using copy constructor
	result->mProperties = mProperties;

	return result;
}

Object *Object::clone(const Id &parent, QHash<Id, Object*> &objHash) const
{
	Object *result = clone(objHash);
	result->setParent(parent);

	return result;
}

void Object::setParent(const Id &parent)
{
	mParent = parent;
}

void Object::removeParent()
{
	mParent = qReal::Id();
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

QVariant Object::property(const QString &name) const
{
	if (mProperties.contains(name)) {
		return mProperties[name];
	} else {
		throw Exception("Object " + mId.toString() + ": requesting nonexistent property " + name);
	}
}

void Object::setTemporaryRemovedLinks(QString const &direction, qReal::IdList const &listValue)
{
	mTemporaryRemovedLinks.insert(direction, listValue);
}

IdList Object::temporaryRemovedLinksAt(QString const &direction) const
{
	return mTemporaryRemovedLinks.value(direction);
}

IdList Object::temporaryRemovedLinks() const
{
	return temporaryRemovedLinksAt("to") << temporaryRemovedLinksAt("from") << temporaryRemovedLinksAt(QString());
}

void Object::removeTemporaryRemovedLinksAt(QString const &direction)
{
	if (mTemporaryRemovedLinks.contains(direction)) {
		mProperties.remove(direction);
	}
}

void Object::removeTemporaryRemovedLinks()
{
	temporaryRemovedLinksAt("from");
	temporaryRemovedLinksAt("to");
	temporaryRemovedLinksAt(QString());
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

	QRegExp *regExp = new QRegExp(name, caseSensitivity);

	if (regExpression) {
		return !properties.filter(*regExp).isEmpty();
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

Id Object::logicalId() const
{
	return mLogicalId;
}

QMapIterator<QString, QVariant> Object::propertiesIterator()
{
	return QMapIterator<QString, QVariant>(mProperties);
}

