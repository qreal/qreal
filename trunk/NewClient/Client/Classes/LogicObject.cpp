#include "LogicObject.h"
#include "../Service/Exception/Exception.h"

using namespace qReal;

using namespace client;

LogicObject::LogicObject(const IdType &id, const IdType &parent) : mId(id)
{
	addParent(parent);
}

LogicObject::LogicObject(const IdType &id) : mId(id)
{
}

void LogicObject::addParent(const IdType &parent)
{
	if (!mParents.contains(parent)) {
		mParents.append(parent);
	} else {
		throw Exception("Object " + mId.toString() + ": adding existing parent " + parent.toString());
	}
}

void LogicObject::removeParent(const IdType &parent)
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

void LogicObject::addChild(const IdType &child)
{
	if (!mChildren.contains(child)) {
		mChildren.append(child);
	} else {
		throw Exception("Object " + mId.toString() + ": adding existing child " + child.toString());
	}
}

void LogicObject::removeChild(const IdType &child)
{
	if (mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Object " + mId.toString() + ": removing nonexistent child " + child.toString());
	}
}

IdTypeList LogicObject::children() const
{
	return mChildren;
}

IdTypeList LogicObject::parents() const
{
	return mParents;
}

void LogicObject::setProperty(const PropertyName &name, const QVariant &value)
{
	mProperties.insert(name,value);
}

QVariant LogicObject::property(const PropertyName &name) const
{
	if (mProperties.contains(name)) {
		return mProperties[name];
	} else {
		throw Exception("Object " + mId.toString() + ": requesting nonexistent property " + name);
	}
}

void LogicObject::removeProperty(const PropertyName &name)
{
	if (mProperties.contains(name)) {
		mProperties.remove(name);
	} else {
		throw Exception("Object " + mId.toString() + ": removing nonexistent property " + name);
	}
}

IdType LogicObject::id() const
{
	return mId;
}

QMapIterator<PropertyName, QVariant> LogicObject::propertiesIterator()
{
	return QMapIterator<PropertyName, QVariant>(mProperties);
}

