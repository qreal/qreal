#pragma once

#include "../qrkernel/roles.h"
#include "commonRepoApi.h"

namespace qrRepo {

class LogicalRepoApi : public CommonRepoApi
{
public:
	virtual ~LogicalRepoApi(){}

	/// virtual for replacing property values that contains input value with new value
	/// @param toReplace - id list that contains ids of elements that properties should be replaced
	/// @param value - input value that should be contained by any property of each element
	/// @param newValue - string representation of value with what property values should be replaced
	virtual void replaceProperties(qReal::IdList const &toReplace, QString const value, QString const newValue) = 0;

	virtual void addChild(qReal::Id const &id, qReal::Id const &child) = 0;
	virtual qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const = 0;

	virtual qReal::IdList outgoingConnections(qReal::Id const &id) const = 0;
	virtual qReal::IdList incomingConnections(qReal::Id const &id) const = 0;
	virtual void connect(qReal::Id const &source, qReal::Id const &destination) = 0;
	virtual void disconnect(qReal::Id const &source, qReal::Id const &destination) = 0;

	virtual qReal::IdList outgoingUsages(qReal::Id const &id) const = 0;
	virtual qReal::IdList incomingUsages(qReal::Id const &id) const = 0;
	virtual void addUsage(qReal::Id const &source, qReal::Id const &destination) = 0;
	virtual void deleteUsage(qReal::Id const &source, qReal::Id const &destination) = 0;

	virtual qReal::IdList logicalElements(qReal::Id const &type) const = 0;
	virtual bool isLogicalElement(qReal::Id const &id) const = 0;

	virtual qReal::IdList elementsByType(QString const &type) const = 0;
	virtual int elementsCount() const = 0;
};
}
