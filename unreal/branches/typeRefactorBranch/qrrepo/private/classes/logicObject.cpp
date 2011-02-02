#include "logicObject.h"
#include "../../../qrgui/kernel/exception/exception.h"

#include <QDebug>

using namespace qrRepo::details;
using namespace qReal;

LogicObject::LogicObject(const NewType &type, const NewType &parent) : mType(type)
{
	addParent(parent);
}

LogicObject::LogicObject(const NewType &type) : mType(type)
{
}

void LogicObject::addParent(const NewType &parent)
{
	if (!mParents.contains(parent)) {
		mParents.append(parent);
	} else {
                throw Exception("Object " + mType.toString() + ": adding existing parent " + parent.toString());
	}
}

void LogicObject::removeParent(const NewType &parent)
{
	if (mParents.contains(parent)) {
		if (mParents.size() != 1) {
			mParents.removeAll(parent);
		} else {
                        throw Exception("Object " + mType.toString() + ": removing the only parent " + parent.toString());
		}
	} else {
                throw Exception("Object " + mType.toString() + ": removing nonexistent parent " + parent.toString());
	}
}

void LogicObject::addChild(const NewType &child)
{
	if (!mChildren.contains(child)) {
		mChildren.append(child);
	} else {
                throw Exception("Object " + mType.toString() + ": adding existing child " + child.toString());
	}
}

void LogicObject::removeChild(const NewType &child)
{
	if (mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
                throw Exception("Object " + mType.toString() + ": removing nonexistent child " + child.toString());
	}
}

TypeList LogicObject::children() const
{
	return mChildren;
}

TypeList LogicObject::parents() const
{
	return mParents;
}

void LogicObject::setProperty(const QString &name, const QVariant &value)
{
	if (value == QVariant()) {
                qDebug() << "Empty QVariant set as a property for " << type().toString();
		qDebug() << ", property name " << name;
		Q_ASSERT(!"Empty QVariant set as a property");
	}
	mProperties.insert(name,value);
}

QVariant LogicObject::property(const QString &name) const
{
	if (mProperties.contains(name)) {
		return mProperties[name];
	} else {
                throw Exception("Object " + mType.toString() + ": requesting nonexistent property " + name);
	}
}

bool LogicObject::hasProperty(const QString &name) const
{
	return mProperties.contains(name);
}

void LogicObject::removeProperty(const QString &name)
{
	if (mProperties.contains(name)) {
		mProperties.remove(name);
	} else {
                throw Exception("Object " + mType.toString() + ": removing nonexistent property " + name);
	}
}

NewType LogicObject::type() const
{
        return mType;
}

QMapIterator<QString, QVariant> LogicObject::propertiesIterator()
{
	return QMapIterator<QString, QVariant>(mProperties);
}

