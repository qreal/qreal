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

#include <qrkernel/roles.h>

namespace qrRepo {

/// Common methods for graphical and logical repository parts.
class CommonRepoApi
{
public:
	/// Destructor.
	virtual ~CommonRepoApi() {}

	/// Replaces property values that contain input value with new value.
	/// @param toReplace - id list that contains ids of elements that properties should be replaced.
	/// @param value - input value that should be contained by any property of each element.
	/// @param newValue - string representation of value with what property values should be replaced.
	virtual void replaceProperties(const qReal::IdList &toReplace, const QString value, const QString newValue) = 0;

	/// Set name of the element.
	virtual void setName(const qReal::Id &id, const QString &name) = 0;

	/// Get name of the element.
	virtual QString name(const qReal::Id &id) const = 0;

	/// Get list of children (in a sense of "container" relation) of the element.
	virtual qReal::IdList children(const qReal::Id &id) const = 0;

	/// Remove child with given Id from list of children. Children/parents
	/// are in a sense of "container" relation.
	virtual void removeChild(const qReal::Id &id, const qReal::Id &child) = 0;

	/// Remove all children from element with given Id. Children/parents
	/// are in a sense of "container" relation.
	virtual void removeChildren(const qReal::Id &id) = 0;

	/// Rearrange existing children so that given child will be in a list
	/// of children just before given other child. Children/parents
	/// are in a sense of "container" relation.
	virtual void stackBefore(const qReal::Id &id, const qReal::Id &child, const qReal::Id &sibling) = 0;

	/// Get a list of all links going from given element.
	virtual qReal::IdList outgoingLinks(const qReal::Id &id) const = 0;

	/// Get a list of all links going to given element.
	virtual qReal::IdList incomingLinks(const qReal::Id &id) const = 0;

	/// Get a list of all links connected to given element.
	virtual qReal::IdList links(const qReal::Id &id) const = 0;

	/// Get an element connected to other side of link.
	/// @param linkId Id of a link.
	/// @param firstNode Id of first element connected to a link.
	/// @returns Id of second element connected to a link.
	virtual qReal::Id otherEntityFromLink(const qReal::Id &linkId, const qReal::Id &firstNode) const = 0;

	/// Get a property of given element.
	/// @param id Id of an element.
	/// @param propertyName Name of a property from which we want to get value.
	/// @returns Property value as QVariant.
	virtual QVariant property(const qReal::Id &id, const QString &propertyName) const = 0;

	/// Get a string property of given element.
	/// @param id Id of an element.
	/// @param propertyName Name of a property from which we want to get value.
	/// @returns Property value as string, if type of property can be converted
	/// to string.
	virtual QString stringProperty(const qReal::Id &id, const QString &propertyName) const = 0;

	/// Set property with given name to given value, creates new property if
	/// there wasn't such property before.
	virtual void setProperty(const qReal::Id &id, const QString &propertyName, const QVariant &value) = 0;

	/// Remove property with given name from given element.
	virtual void removeProperty(const qReal::Id &id, const QString &propertyName) = 0;

	/// Check that property with given name exists in a given element.
	virtual bool hasProperty(const qReal::Id &id, const QString &propertyName) const = 0;

	virtual QMapIterator<QString, QVariant> propertiesIterator(const qReal::Id &id) const = 0;

	virtual void setBackReference(const qReal::Id &id, const qReal::Id &reference) const = 0;
	virtual void removeBackReference(const qReal::Id &id, const qReal::Id &reference) const = 0;

	/// Check that given element exists in a repository.
	virtual bool exist(const qReal::Id &id) const = 0;

	/// Remove given element from repository.
	virtual void removeElement(const qReal::Id &id) = 0;

	/// Get source node for a link, meaningless for nodes.
	virtual qReal::Id from(const qReal::Id &id) const = 0;

	/// Get destination node for a link, meaningless for nodes.
	virtual qReal::Id to(const qReal::Id &id) const = 0;

	/// Set source node for a link, meaningless for nodes.
	virtual void setFrom(const qReal::Id &id, const qReal::Id &from) = 0;

	/// Set destination node for a link, meaningless for nodes.
	virtual void setTo(const qReal::Id &id, const qReal::Id &to) = 0;

	/// Get parent of given node. Children/parents are in a sense of
	/// "container" relation.
	virtual qReal::Id parent(const qReal::Id &id) const = 0;

	/// Set parent of given node. Children/parents are in a sense of
	/// "container" relation.
	virtual void setParent(const qReal::Id &id, const qReal::Id &parent) = 0;
};

}
