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

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

namespace qrRepo {
namespace details {

/// Represents part of a graphical object which has its own properties, like label or pin.
class GraphicalPart
{
public:
	/// Creates empty graphical part.
	GraphicalPart();

	/// Deserializing constructor.
	/// @param element - root of XML DOM subtree with serialized graphical part.
	explicit GraphicalPart(const QDomElement &element);

	/// Returns value of a property with given name or throws an exception if there is no such property in this part.
	QVariant property(const QString &name) const;

	/// Sets the value of given property. If a property already exists, its value will be overwritten,
	/// otherwise new property will be created with given value.
	void setProperty(const QString &name, const QVariant &value);

	/// Creates deep copy of object.
	GraphicalPart *clone() const;

	/// Serializes contents of an object to XML DOM subtree.
	/// @param index - index of a part in its parent graphical object.
	/// @param document - document to which will belong created subtree.
	QDomElement serialize(int index, QDomDocument &document) const;

private:
	/// A list of properties in a form of pairs (name, value).
	QMap<QString, QVariant> mProperties;
};

}
}
