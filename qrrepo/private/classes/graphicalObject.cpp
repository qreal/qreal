#include "graphicalObject.h"

#include "../../../qrkernel/exception/exception.h"

using namespace qrRepo::details;
using namespace qReal;

GraphicalObject::GraphicalObject(qReal::Id const &id, qReal::Id const &parent, qReal::Id const &logicalId)
	: Object(id, parent)
	, mLogicalId(logicalId)
{
}

GraphicalObject::~GraphicalObject()
{
	qDeleteAll(mGraphicalParts.values());
}

void GraphicalObject::createGraphicalPart(int index)
{
	GraphicalPart * const part = new GraphicalPart();
	mGraphicalParts.insert(index, part);
}

QVariant GraphicalObject::graphicalPartProperty(int index, QString const &name) const
{
	if (!mGraphicalParts.contains(index)) {
		throw Exception("Tryng to get property of non-existing graphical part");
	}

	return mGraphicalParts[index]->property(name);
}

void GraphicalObject::setGraphicalPartProperty(int index, QString const &name, QVariant const &value)
{
	if (!mGraphicalParts.contains(index)) {
		throw Exception("Tryng to set property of non-existing graphical part");
	}

	mGraphicalParts[index]->setProperty(name, value);
}

Object *GraphicalObject::createClone() const
{
	GraphicalObject * const clone = new GraphicalObject(mId.sameTypeId(), mParent, mLogicalId);

	QHash<int, GraphicalPart *>::const_iterator i = mGraphicalParts.constBegin();
	while (i != mGraphicalParts.constEnd()) {
		GraphicalPart * const partClone = i.value()->clone();
		clone->mGraphicalParts.insert(i.key(), partClone);
		++i;
	}

	return clone;
}

Id GraphicalObject::logicalId() const
{
	return mLogicalId;
}

bool GraphicalObject::isLogicalObject() const
{
	return false;
}
