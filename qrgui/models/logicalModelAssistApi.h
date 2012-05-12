#pragma once
#include "../../qrkernel/ids.h"
#include "details/logicalModel.h"
#include "details/modelsAssistApi.h"
#include "../toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"

namespace qReal {

class EditorManager;
class ConstraintsManager;

namespace models {

namespace details {
class LogicalModel;
}

class LogicalModelAssistApi : public QObject, public qReal::LogicalModelAssistInterface
{
	Q_OBJECT

public:
	LogicalModelAssistApi(details::LogicalModel &logicalModel, EditorManager const &editorManager, ConstraintsManager const &constraintsManager);
	virtual ~LogicalModelAssistApi() {}
	EditorManager const &editorManager() const;
	ConstraintsManager const &constraintsManager() const;
	qrRepo::LogicalRepoApi const &logicalRepoApi() const;
	qrRepo::LogicalRepoApi &mutableLogicalRepoApi();
	Id createElement(Id const &parent, Id const &type);
	Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name, QPointF const &position);
	IdList children(Id const &element) const;
	void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF());

	void connect(Id const &source, Id const &destination);
	void disconnect(Id const &source, Id const &destination);
	void addUsage(Id const &source, Id const &destination);
	void deleteUsage(Id const &source, Id const &destination);
	void createConnected(Id const &sourceElement, Id const &elementType);
	void createUsed(Id const &sourceElement, Id const &elementType);
	Id createConnectedElement(Id const &source, Id const &elementType);
	IdList diagramsAbleToBeConnectedTo(Id const &element) const;
	IdList diagramsAbleToBeUsedIn(Id const &element) const;
	virtual void stackBefore(Id const &element, Id const &sibling);

	void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName);
	QVariant propertyByRoleName(Id const &elem, QString const &roleName) const;

	bool isLogicalId(Id const &id) const;

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

public slots:
	void propertyChangedSlot(Id const &elem);//qwerty_doubt
	void parentChangedSlot(IdList const &elements);
	void nameChangedSlot(Id const &element);
	void addedElementToModelSlot(Id const &element);
	void toChangedSlot(Id const &element);//qwerty_temp
	void fromChangedSlot(Id const &element);

signals:
	void propertyChanged(Id const &elem);//qwerty_doubt
	void parentChanged(IdList const &elements);
	void nameChanged(Id const &element);
	void addedElementToModel(Id const &element);
	void toChanged(Id const &element);//qwerty_temp
	void fromChanged(Id const &element);

private:
	details::ModelsAssistApi mModelsAssistApi;
	details::LogicalModel &mLogicalModel;

	LogicalModelAssistApi(LogicalModelAssistApi const &);  // Copying is forbidden
	LogicalModelAssistApi& operator =(LogicalModelAssistApi const &); // Assignment is forbidden also

	IdList diagramsFromList(IdList const &list) const;
};
}
}
