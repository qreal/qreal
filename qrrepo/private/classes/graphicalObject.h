/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
	GraphicalObject(const qReal::Id &id, const qReal::Id &parent, const qReal::Id &logicalId);

	/// Deserializing constructor.
	/// @param element - root of XML DOM subtree with serialized object.
	explicit GraphicalObject(const QDomElement &element);

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
	QVariant graphicalPartProperty(int index, const QString &name) const;

	/// Sets the value of graphical part property. If a property already exists, its value will be overwritten,
	/// otherwise new property will be created with given value.
	/// @param index - index of a graphical part.
	/// @param name - name of a property which value we want to set.
	/// @param value - new value of a property.
	void setGraphicalPartProperty(int index, const QString &name, const QVariant &value);

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
