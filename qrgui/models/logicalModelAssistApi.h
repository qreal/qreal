#pragma once
#include "../../qrkernel/ids.h"
#include "details/logicalModel.h"
#include "details/modelsAssistApi.h"
#include "../toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "../pluginManager/proxyEditorManager.h"

namespace qReal {

class EditorManager;
class EditorManagerInterface;

namespace models {

namespace details {
class LogicalModel;
}

class LogicalModelAssistApi : public qReal::LogicalModelAssistInterface
{
public:
	LogicalModelAssistApi(details::LogicalModel &logicalModel, EditorManagerInterface const &editorManagerInterface);
	virtual ~LogicalModelAssistApi() {}
	EditorManagerInterface const &editorManagerInterface() const;
	qrRepo::LogicalRepoApi const &logicalRepoApi() const;
	qrRepo::LogicalRepoApi &mutableLogicalRepoApi();
	Id createElement(Id const &parent, Id const &type);
	Id createElement(Id const &parent, Id const &id
			, bool isFromLogicalModel, QString const &name
			, QPointF const &position, Id const &preferedLogicalId = Id());
	IdList children(Id const &element) const;
	void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF());

	void addExplosion(Id const &source, Id const &destination);
	void removeExplosion(Id const &source, Id const &destination);
	void addUsage(Id const &source, Id const &destination);
	void deleteUsage(Id const &source, Id const &destination);
	Id createConnected(Id const &sourceElement, Id const &elementType);
	Id createUsed(Id const &sourceElement, Id const &elementType);
	Id createConnectedElement(Id const &source, Id const &elementType);
	IdList diagramsAbleToBeConnectedTo(Id const &element) const;
	IdList diagramsAbleToBeUsedIn(Id const &element) const;
	virtual void stackBefore(Id const &element, Id const &sibling);

	virtual QVariant property(Id const &id, QString const &name) const;
	virtual void setProperty(Id const &id, QString const &name, QVariant const &value);

	virtual void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName);
	virtual QVariant propertyByRoleName(Id const &elem, QString const &roleName) const;

	bool isLogicalId(Id const &id) const;

	void removeReferencesTo(Id const &id);
	void removeReferencesFrom(Id const &id);
	void removeReference(Id const &id, Id const &reference);

	void setTo(Id const &elem, Id const &newValue);
	Id to(Id const &elem) const;

	void setFrom(Id const &elem, Id const &newValue);
	Id from(Id const &elem) const;

	QModelIndex indexById(Id const &id) const;
	Id idByIndex(QModelIndex const &index) const;
	QPersistentModelIndex rootIndex() const;
	Id rootId() const;

	bool hasRootDiagrams() const;
	int childrenOfRootDiagram() const;
	int childrenOfDiagram(const Id &parent) const;

	void removeElement(Id const &logicalId);

private:
	details::ModelsAssistApi mModelsAssistApi;
	details::LogicalModel &mLogicalModel;
	EditorManagerInterface const &mEditorManager;

	LogicalModelAssistApi(LogicalModelAssistApi const &);  // Copying is forbidden
	LogicalModelAssistApi& operator =(LogicalModelAssistApi const &); // Assignment is forbidden also

	IdList diagramsFromList(IdList const &list) const;
};
}
}
