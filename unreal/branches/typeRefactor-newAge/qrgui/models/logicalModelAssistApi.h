#pragma once
#include "../kernel/ids.h"
#include "details/logicalModel.h"
#include "details/modelsAssistApi.h"

namespace qReal {

class EditorManager;

namespace models {

namespace details {
class LogicalModel;
}

class LogicalModelAssistApi : public details::ModelsAssistApi
{
public:
	LogicalModelAssistApi(details::LogicalModel &logicalModel, EditorManager const &editorManager);
	qrRepo::LogicalRepoApi const &logicalRepoApi() const;
	virtual Id createElement(Id const &parent, NewType const &type);
	virtual Id createElement(Id const &parent, Id const &id, NewType const &type, bool isFromLogicalModel, QString const &name, QPointF const &position);
	virtual IdList children(Id const &element) const;
	virtual void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF());

	NewType type(Id const &id) const;
	void connect(Id const &source, Id const &destination);
	void disconnect(Id const &source, Id const &destination);
	void addUsage(Id const &source, Id const &destination);
	void deleteUsage(Id const &source, Id const &destination);
	void createConnected(Id const &sourceElement, NewType const &elementType);
	void createUsed(Id const &sourceElement, Id const &elementType);
	Id createConnectedElement(Id const &source, NewType const &elementType);
	TypeList diagramsAbleToBeConnectedTo(Id const &element) const;
	TypeList diagramsAbleToBeUsedIn(Id const &element) const;

	void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName);
	QVariant propertyByRoleName(Id const &elem, QString const &roleName) const;

	bool isLogicalId(Id const &id) const;


private:
	details::LogicalModel &mLogicalModel;
	LogicalModelAssistApi(LogicalModelAssistApi const &);  // Copying is forbidden

	LogicalModelAssistApi& operator =(LogicalModelAssistApi const &); // Assignment is forbidden also

	IdList diagramsFromList(IdList const &list) const;
};
}
}
