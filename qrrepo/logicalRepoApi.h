#pragma once

#include "../qrkernel/roles.h"
#include "commonRepoApi.h"

namespace qrRepo {

class LogicalRepoApi : public CommonRepoApi
{
public:
	virtual ~LogicalRepoApi(){}

	virtual void addChild(qReal::Id const &id, qReal::Id const &child) = 0;
	virtual qReal::Id otherEntityFromLink(qReal::Id const &linkId, qReal::Id const &firstNode) const = 0;
	virtual void stackBefore(qReal::Id const &id, qReal::Id const &child, qReal::Id const &sibling) = 0;

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
