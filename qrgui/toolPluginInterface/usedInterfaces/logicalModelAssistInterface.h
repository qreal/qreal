#pragma once

#include "../../../qrkernel/ids.h"
#include "../../../qrrepo/logicalRepoApi.h"

#include "details/modelsAssistInterface.h"

namespace qReal {

class LogicalModelAssistInterface: public details::ModelsAssistInterface
{
public:
	/// Returns reference to a constant logical repository.
	virtual qrRepo::LogicalRepoApi const &logicalRepoApi() const = 0;

	/// Returns reference to a mutable logical repository.
	virtual qrRepo::LogicalRepoApi &mutableLogicalRepoApi() = 0;

	virtual Id createElement(Id const &parent, Id const &type) = 0;
	virtual Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position) = 0;
	virtual IdList children(Id const &element) const = 0;
	virtual void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF()) = 0;

	virtual void connect(Id const &source, Id const &destination) = 0;
	virtual void disconnect(Id const &source, Id const &destination) = 0;
	virtual void addUsage(Id const &source, Id const &destination) = 0;
	virtual void deleteUsage(Id const &source, Id const &destination) = 0;
	virtual void createConnected(Id const &sourceElement, Id const &elementType) = 0;
	virtual void createUsed(Id const &sourceElement, Id const &elementType) = 0;
	virtual Id createConnectedElement(Id const &source, Id const &elementType) = 0;
	virtual IdList diagramsAbleToBeConnectedTo(Id const &element) const = 0;
	virtual IdList diagramsAbleToBeUsedIn(Id const &element) const = 0;

	virtual void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName) = 0;
	virtual QVariant propertyByRoleName(Id const &elem, QString const &roleName) const = 0;

	virtual bool isLogicalId(Id const &id) const = 0;

	virtual void removeReferencesTo(Id const &id) const = 0;
	virtual void removeReferencesFrom(Id const &id) const = 0;
	virtual void removeReference(Id const &id, Id const &reference) const = 0;
};

}
