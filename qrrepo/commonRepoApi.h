#pragma once

#include "../qrkernel/roles.h"

namespace qrRepo {

class CommonRepoApi
{
public:
	virtual ~CommonRepoApi() {}

	/// virtual for replacing property values that contains input value with new value
	/// @param toReplace - id list that contains ids of elements that properties should be replaced
	/// @param value - input value that should be contained by any property of each element
	/// @param newValue - string representation of value with what property values should be replaced
	virtual void replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue) = 0;

	virtual void setName(qReal::Id const &id, QString const &name) = 0;
	virtual QString name(qReal::Id const &id) const = 0;

	virtual qReal::IdList children(qReal::Id const &id) const = 0;
	virtual void removeChild(qReal::Id const &id, qReal::Id const &child) = 0;
	virtual void removeChildren(qReal::Id const &id) = 0;

	/// Stacks element child before sibling (element id shold be parent of them both)
	virtual void stackBefore(qReal::Id const &id, qReal::Id const &child, qReal::Id const &sibling) = 0;

	virtual qReal::IdList outgoingLinks(qReal::Id const &id) const = 0;
	virtual qReal::IdList incomingLinks(qReal::Id const &id) const = 0;
	virtual qReal::IdList links(qReal::Id const &id) const = 0;
	virtual qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const = 0;

	virtual QVariant property(qReal::Id const &id, QString const &propertyName) const = 0;
	virtual QString stringProperty(qReal::Id const &id, QString const &propertyName) const = 0;
	virtual void setProperty(qReal::Id const &id, QString const &propertyName, QVariant const &value) const = 0;
	virtual void removeProperty(qReal::Id const &id, QString const &propertyName) = 0;
	virtual bool hasProperty(qReal::Id const &id, QString const &propertyName) const = 0;
	virtual QMapIterator<QString, QVariant> propertiesIterator(qReal::Id const &id) const = 0;

	virtual bool exist(qReal::Id const &id) const = 0;
	virtual void removeElement(qReal::Id const &id) = 0;

	virtual qReal::Id from(qReal::Id const &id) const = 0;
	virtual qReal::Id to(qReal::Id const &id) const = 0;

	virtual void setFrom(qReal::Id const &id, qReal::Id const &from) = 0;
	virtual void setTo(qReal::Id const &id, qReal::Id const &to) = 0;

	virtual qReal::Id parent(qReal::Id const &id) const = 0;
	virtual void setParent(qReal::Id const &id, qReal::Id const &parent) = 0;

	virtual QString typeName(qReal::Id const &id) const = 0;
};

}
