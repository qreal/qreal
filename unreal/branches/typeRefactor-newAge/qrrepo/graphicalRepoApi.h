#pragma once

#include "../qrgui/kernel/roles.h"
#include "../qrgui/kernel/newType.h"
#include "commonRepoApi.h"

namespace qrRepo {

class GraphicalRepoApi : public CommonRepoApi
{
public:
	virtual ~GraphicalRepoApi(){}

	virtual void addChild(qReal::Id const &id, qReal::Id const &logicalId, qReal::Id const &child, qReal::NewType const &type) = 0;

	virtual qReal::NewType type(qReal::Id const &id) const;
	virtual double fromPort(qReal::Id const &id) const = 0;
	virtual void setFromPort(qReal::Id const &id, double fromPort) = 0;

	virtual double toPort(qReal::Id const &id) const = 0;
	virtual void setToPort(qReal::Id const &id, double toPort) = 0;

	virtual QVariant position(qReal::Id const &id) const = 0;
	virtual QVariant configuration(qReal::Id const &id) const = 0;

	virtual void setPosition(qReal::Id const &id, QVariant const &position) = 0;
	virtual void setConfiguration(qReal::Id const &id, QVariant const &configuration) = 0;

	virtual qReal::IdList graphicalElements(qReal::NewType const &type) const = 0;

	virtual bool isGraphicalElement(qReal::Id const &id) const = 0;
	virtual qReal::Id logicalId(qReal::Id const &id) const = 0;

	virtual qReal::IdList temporaryRemovedLinksAt(qReal::Id const &id, QString const &direction) const = 0;
	virtual void setTemporaryRemovedLinks(qReal::Id const &id, qReal::IdList const &value, QString const &direction) = 0;
	virtual void removeTemporaryRemovedLinks(qReal::Id const &id) = 0;
};

}
