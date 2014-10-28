#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {

class LogicalModelAssistInterface: public details::ModelsAssistInterface
{
public:
	/// Returns reference to a constant logical repository.
	virtual qrRepo::LogicalRepoApi const &logicalRepoApi() const = 0;

	/// Returns reference to a mutable logical repository.
	virtual qrRepo::LogicalRepoApi &mutableLogicalRepoApi() = 0;

	virtual Id createElement(Id const &parent, Id const &type) = 0;
	virtual Id createElement(Id const &parent, Id const &id
			, bool isFromLogicalModel, QString const &name
			, QPointF const &position, Id const &preferedLogicalId = Id()) = 0;
	virtual IdList children(Id const &element) const = 0;
	virtual void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF()) = 0;

	virtual void addExplosion(Id const &source, Id const &destination) = 0;
	virtual void removeExplosion(Id const &source, Id const &destination) = 0;

	virtual void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName) = 0;
	virtual QVariant propertyByRoleName(Id const &elem, QString const &roleName) const = 0;

	virtual bool isLogicalId(Id const &id) const = 0;

	virtual void removeReferencesTo(Id const &id) = 0;
	virtual void removeReferencesFrom(Id const &id) = 0;
	virtual void removeReference(Id const &id, Id const &reference) = 0;
};

}
