#include "object.h"
#include "../../../qrgui/kernel/exception/exception.h"

#include <QDebug>

using namespace qrRepo::details;
using namespace qReal;

Object::Object(const Id &id, const Id &parent) : mId(id)
{
	addParent(parent);
}

Object::Object(const Id &id) : mId(id)
{
}

void Object::addParent(const Id &parent)
{
	if (!mParents.contains(parent)) {
		mParents.append(parent);
	} else {
		throw Exception("Object " + mId.toString() + ": adding existing parent " + parent.toString());
	}
}

void Object::removeParent(const Id &parent)
{
	if (mParents.contains(parent)) {
		if (mParents.size() != 1) {
			mParents.removeAll(parent);
		} else {
			throw Exception("Object " + mId.toString() + ": removing the only parent " + parent.toString());
		}
	} else {
		throw Exception("Object " + mId.toString() + ": removing nonexistent parent " + parent.toString());
	}
}

void Object::addChild(const Id &child)
{
	if (!mChildren.contains(child)) {
		mChildren.append(child);
	} else {
		throw Exception("Object " + mId.toString() + ": adding existing child " + child.toString());
	}
}

void Object::removeChild(const Id &child)
{
	if (mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Object " + mId.toString() + ": removing nonexistent child " + child.toString());
	}
}

IdList Object::children() const
{
	return mChildren;
}

IdList Object::parents() const
{
	return mParents;
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

bool Object::hasProperty(const QString &name) const
{
	return mProperties.contains(name);
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

QMapIterator<QString, QVariant> Object::propertiesIterator()
{
	return QMapIterator<QString, QVariant>(mProperties);
}

