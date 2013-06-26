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

	virtual qReal::IdList outgoingExplosions(qReal::Id const &id) const = 0;
	virtual qReal::IdList incomingExplosions(qReal::Id const &id) const = 0;
	virtual void addExplosion(qReal::Id const &source, qReal::Id const &destination) = 0;
	virtual void removeExplosion(qReal::Id const &source, qReal::Id const &destination) = 0;

	virtual qReal::IdList logicalElements(qReal::Id const &type) const = 0;
	virtual bool isLogicalElement(qReal::Id const &id) const = 0;

	virtual qReal::IdList elementsByType(QString const &type, bool sensitivity = false
			, bool regExpression = false) const = 0;
	virtual int elementsCount() const = 0;
};
}
