#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>

#include "qrgui/plugins/toolPluginInterface/usedInterfaces/details/modelsAssistInterface.h"

namespace qReal {

class LogicalModelAssistInterface: public details::ModelsAssistInterface
{
public:
	/// Returns reference to a constant logical repository.
	virtual const qrRepo::LogicalRepoApi &logicalRepoApi() const = 0;

	/// Returns reference to a mutable logical repository.
	virtual qrRepo::LogicalRepoApi &mutableLogicalRepoApi() = 0;

	virtual Id createElement(const Id &parent, const Id &type) = 0;
	virtual Id createElement(const Id &parent, const Id &id
			, bool isFromLogicalModel, const QString &name
			, const QPointF &position, const Id &preferedLogicalId = Id()) = 0;
	virtual IdList children(const Id &element) const = 0;
	virtual void changeParent(const Id &element, const Id &parent, const QPointF &position = QPointF()) = 0;

	virtual void addExplosion(const Id &source, const Id &destination) = 0;
	virtual void removeExplosion(const Id &source, const Id &destination) = 0;

	virtual void setPropertyByRoleName(const Id &elem, const QVariant &newValue, const QString &roleName) = 0;
	virtual QVariant propertyByRoleName(const Id &elem, const QString &roleName) const = 0;

	virtual bool isLogicalId(const Id &id) const = 0;

	virtual void removeReferencesTo(const Id &id) = 0;
	virtual void removeReferencesFrom(const Id &id) = 0;
	virtual void removeReference(const Id &id, const Id &reference) = 0;
};

}
