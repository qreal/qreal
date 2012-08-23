#pragma once

#include "../qrkernel/roles.h"

namespace qrRepo {

/// Common methods for graphical and logical repository parts.
class CommonRepoApi
{
public:
	/// Destructor.
	virtual ~CommonRepoApi() {}

	/// virtual for replacing property values that contains input value with new value
	/// @param toReplace - id list that contains ids of elements that properties should be replaced
	/// @param value - input value that should be contained by any property of each element
	/// @param newValue - string representation of value with what property values should be replaced
	virtual void replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue) = 0;

	virtual void setName(qReal::Id const &id, QString const &name) = 0;

	/// Get name of the element.
	virtual QString name(qReal::Id const &id) const = 0;

	/// Get list of children (in a sense of "container" relation) of the element.
	virtual qReal::IdList children(qReal::Id const &id) const = 0;

	/// Remove child with given Id from list of children. Children/parents
	/// are in a sense of "container" relation.
	virtual void removeChild(qReal::Id const &id, qReal::Id const &child) = 0;

	/// Remove all children from element with given Id. Children/parents
	/// are in a sense of "container" relation.
	virtual void removeChildren(qReal::Id const &id) = 0;

	/// Rearrange existing children so that given child will be in a list
	/// of children just before given other child. Children/parents
	/// are in a sense of "container" relation.
	virtual void stackBefore(qReal::Id const &id, qReal::Id const &child, qReal::Id const &sibling) = 0;

	/// Get a list of all links going from given element.
	virtual qReal::IdList outgoingLinks(qReal::Id const &id) const = 0;

	/// Get a list of all links going to given element.
	virtual qReal::IdList incomingLinks(qReal::Id const &id) const = 0;

	/// Get a list of all links connected to given element.
	virtual qReal::IdList links(qReal::Id const &id) const = 0;

	/// Get an element connected to other side of link.
	/// @param linkId Id of a link.
	/// @param firstNode Id of first element connected to a link.
	/// @returns Id of second element connected to a link.
	virtual qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const = 0;

	/// Get a property of given element.
	/// @param id Id of an element.
	/// @param propertyName Name of a property from which we want to get value.
	/// @returns Property value as QVariant.
	virtual QVariant property(qReal::Id const &id, QString const &propertyName) const = 0;

	/// Get a string property of given element.
	/// @param id Id of an element.
	/// @param propertyName Name of a property from which we want to get value.
	/// @returns Property value as string, if type of property can be converted
	/// to string.
	virtual QString stringProperty(qReal::Id const &id, QString const &propertyName) const = 0;

	/// Set property with given name to given value, creates new property if
	/// there wasn't such property before.
	virtual void setProperty(qReal::Id const &id, QString const &propertyName, QVariant const &value) = 0;

	/// Remove property with given name from given element.
	virtual void removeProperty(qReal::Id const &id, QString const &propertyName) = 0;

	/// Check that property with given name exists in a given element.
	virtual bool hasProperty(qReal::Id const &id, QString const &propertyName) const = 0;
	virtual QMapIterator<QString, QVariant> propertiesIterator(qReal::Id const &id) const = 0;

	virtual void setBackReference(qReal::Id const &id, qReal::Id const &reference) const = 0;
	virtual void removeBackReference(qReal::Id const &id, qReal::Id const &reference) const = 0;

	/// Check that given element exists in a repository.
	virtual bool exist(qReal::Id const &id) const = 0;

	/// Remove given element from repository.
	virtual void removeElement(qReal::Id const &id) = 0;

	/// Get source node for a link, meaningless for nodes.
	virtual qReal::Id from(qReal::Id const &id) const = 0;

	/// Get destination node for a link, meaningless for nodes.
	virtual qReal::Id to(qReal::Id const &id) const = 0;

	/// Set source node for a link, meaningless for nodes.
	virtual void setFrom(qReal::Id const &id, qReal::Id const &from) = 0;

	/// Set destination node for a link, meaningless for nodes.
	virtual void setTo(qReal::Id const &id, qReal::Id const &to) = 0;

	/// Get parent of given node. Children/parents are in a sense of
	/// "container" relation.
	virtual qReal::Id parent(qReal::Id const &id) const = 0;

	/// Set parent of given node. Children/parents are in a sense of
	/// "container" relation.
	virtual void setParent(qReal::Id const &id, qReal::Id const &parent) = 0;
};

}
