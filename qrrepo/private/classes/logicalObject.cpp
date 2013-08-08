#include "logicalObject.h"

using namespace qrRepo::details;

LogicalObject::LogicalObject(qReal::Id const &id)
	: Object(id)
{
}

LogicalObject::LogicalObject(QDomElement const &element)
	: Object(element)
{
}

Object *LogicalObject::createClone() const
{
	return new LogicalObject(mId.sameTypeId());
}

bool LogicalObject::isLogicalObject() const
{
	return true;
}
