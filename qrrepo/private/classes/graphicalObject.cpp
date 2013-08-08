#include "graphicalObject.h"

#include "../../../qrkernel/exception/exception.h"
#include "../valuesSerializer.h"

using namespace qrRepo::details;
using namespace qReal;

GraphicalObject::GraphicalObject(qReal::Id const &id, Id const &parent, qReal::Id const &logicalId)
	: Object(id)
	, mLogicalId(logicalId)
{
	mParent = parent;
}

GraphicalObject::GraphicalObject(QDomElement const &element)
	: Object(element)
{
	mLogicalId = Id::loadFromString(element.attribute("logicalId"));
	if (mLogicalId == Id()) {
		throw Exception("Logical id not found for graphical object");
	}

	QDomElement graphicalParts = element.firstChildElement("graphicalParts");
	if (!graphicalParts.isNull()) {
		QDomElement part = graphicalParts.firstChildElement();
		while (!part.isNull()) {
			GraphicalPart * const deserializedPart = new GraphicalPart(part);

			QString const indexString = part.attribute("index");
			if (indexString.isEmpty()) {
				throw Exception("No \"index\" attribute in graphical part");
			}

			int index = indexString.toInt();
			mGraphicalParts.insert(index, deserializedPart);

			part = part.nextSiblingElement();
		}
	}
}

GraphicalObject::~GraphicalObject()
{
	qDeleteAll(mGraphicalParts.values());
}

QDomElement GraphicalObject::serialize(QDomDocument &document) const
{
	QDomElement result = Object::serialize(document);
	result.setAttribute("logicalId", mLogicalId.toString());

	QDomElement graphicalParts = document.createElement("graphicalParts");
	result.appendChild(graphicalParts);

	QHash<int, GraphicalPart *>::const_iterator i = mGraphicalParts.constBegin();
	while (i != mGraphicalParts.constEnd()) {
		QDomElement graphicalPart = i.value()->serialize(i.key(), document);
		graphicalParts.appendChild(graphicalPart);
		++i;
	}

	return result;
}

void GraphicalObject::createGraphicalPart(int index)
{
	if (mGraphicalParts.contains(index)) {
		throw Exception("Part with that index already exists");
	}

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
