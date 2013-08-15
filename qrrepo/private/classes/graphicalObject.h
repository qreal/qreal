#pragma once

#include <QtCore/QHash>
#include <QtCore/QVariant>

#include "object.h"
#include "graphicalPart.h"

namespace qrRepo {
namespace details {

/// Represents graphical object in repository.
class GraphicalObject : public Object
{
public:
	/// Constructor.
	/// @param id - id of a new object.
	/// @param parent - id of a parent of new object. Graphical object must always have a parent (for example, ROOT_ID).
	/// @param logicalId - id of corresponding logical object. Graphical object must always represent a logical object.
	GraphicalObject(qReal::Id const &id, qReal::Id const &parent, qReal::Id const &logicalId);

	/// Deserializing constructor.
	/// @param element - root of XML DOM subtree with serialized object.
	explicit GraphicalObject(QDomElement const &element);

	virtual ~GraphicalObject();

	/// Returns id of corresponding logical object.
	qReal::Id logicalId() const;

	// Override.
	virtual bool isLogicalObject() const;

	// Override.
	virtual QDomElement serialize(QDomDocument &document) const;

	/// Creates empty graphical part with given index inside this object.
	void createGraphicalPart(int index);

	/// Returns a list of indexes of graphical parts.
	QList<int> graphicalParts() const;

	/// Returns the value of graphical part property.
	/// @param index - index of a graphical part.
	/// @param name - name of a property which value we want to get.
	QVariant graphicalPartProperty(int index, QString const &name) const;

	/// Sets the value of graphical part property. If a property already exists, its value will be overwritten,
	/// otherwise new property will be created with given value.
	/// @param index - index of a graphical part.
	/// @param name - name of a property which value we want to set.
	/// @param value - new value of a property.
	void setGraphicalPartProperty(int index, QString const &name, QVariant const &value);

protected:
	// Override.
	virtual Object *createClone() const;

private:
	/// Id of logical object corresponding to this graphical object.
	qReal::Id mLogicalId;

	/// A list of graphical parts with their indexes.
	QHash<int, GraphicalPart *> mGraphicalParts;  // Has ownership.
};

}
}
