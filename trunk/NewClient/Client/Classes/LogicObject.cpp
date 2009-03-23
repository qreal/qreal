#include "LogicObject.h"
#include "Exception.h"

using namespace qReal;

using namespace client;

LogicObject::LogicObject( IdType id, IdType parent ) : mId(id)
{
	addParent(parent);
}

void LogicObject::addParent( IdType parent )
{
	if (!mParents.contains(parent)) {
		mParents.append(parent);
	} else {
		throw Exception("Object " + mId + ": adding existing parent " + parent);
	}
}

void LogicObject::removeParent( IdType parent )
{
	if (mParents.contains(parent)) {
		if (mParents.size()!=1) {
			mParents.removeAll(parent);
		} else {
			throw Exception("Object " + mId + ": removing the only parent " + parent);
		}
	} else {
		throw Exception("Object " + mId + ": removing nonexistent parent " + parent);
	}
}

void LogicObject::addChild( IdType child )
{
	if (!mChildren.contains(child)) {
		mChildren.append(child);
	} else {
		throw Exception("Object " + mId + ": adding existing child " + child);
	}
}

void LogicObject::removeChild( IdType child )
{
	if (mChildren.contains(child)) {
		mChildren.removeAll(child);
	} else {
		throw Exception("Object " + mId + ": removing nonexistent child " + child);
	}
}

IdTypeList LogicObject::children()
{
	return mChildren;
}

IdTypeList LogicObject::parents()
{
	return mParents;
}

void LogicObject::setProperty( PropertyType type, QVariant value )
{
	mProperties.insert(type,value);
}

QVariant LogicObject::property( PropertyType type )
{
	if (mProperties.contains(type)) {
		return mProperties[type];
	} else {
		throw Exception("Object " + mId + ": requesting nonexistent property " + type);
	}
}