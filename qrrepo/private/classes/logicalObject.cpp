#include "logicalObject.h"

using namespace qrRepo::details;

LogicalObject::LogicalObject(qReal::Id const &id)
	: Object(id)
{
}

LogicalObject::LogicalObject(const qReal::Id &id, const qReal::Id &parent)
	: Object(id, parent)
{
}

Object *LogicalObject::createClone() const
{
	return new LogicalObject(mId.sameTypeId(), mParent);
}

bool LogicalObject::isLogicalObject() const
{
	return true;
}
