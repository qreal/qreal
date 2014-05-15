#pragma once

#include <qrkernel/ids.h>

#include "models/details/logicalModel.h"
#include "models/details/modelsAssistApi.h"
#include "models/details/exploser.h"
#include "toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"
#include "pluginManager/proxyEditorManager.h"

namespace qReal {

class EditorManager;
class EditorManagerInterface;

namespace models {

class LogicalModelAssistApi : public qReal::LogicalModelAssistInterface
{
public:
	LogicalModelAssistApi(details::LogicalModel &logicalModel, EditorManagerInterface const &editorManagerInterface);
	virtual ~LogicalModelAssistApi();

	EditorManagerInterface const &editorManagerInterface() const;
	Exploser &exploser();

	virtual qrRepo::LogicalRepoApi const &logicalRepoApi() const;
	virtual qrRepo::LogicalRepoApi &mutableLogicalRepoApi();
	virtual Id createElement(Id const &parent, Id const &type);
	virtual Id createElement(Id const &parent, Id const &id
			, bool isFromLogicalModel, QString const &name
			, QPointF const &position, Id const &preferedLogicalId = Id());

	virtual IdList children(Id const &element) const;
	virtual void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF());

	virtual void addExplosion(Id const &source, Id const &destination);
	virtual void removeExplosion(Id const &source, Id const &destination);

	virtual void stackBefore(Id const &element, Id const &sibling);

	virtual void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName);
	virtual QVariant propertyByRoleName(Id const &elem, QString const &roleName) const;

	virtual bool isLogicalId(Id const &id) const;

	virtual void removeReferencesTo(Id const &id);
	virtual void removeReferencesFrom(Id const &id);
	virtual void removeReference(Id const &id, Id const &reference);

	virtual void setName(Id const &elem, QString const &newValue);
	virtual QString name(Id const &elem) const;

	virtual void setTo(Id const &elem, Id const &newValue);
	virtual Id to(Id const &elem) const;

	virtual void setFrom(Id const &elem, Id const &newValue);
	virtual Id from(Id const &elem) const;

	virtual QModelIndex indexById(Id const &id) const;
	virtual Id idByIndex(QModelIndex const &index) const;
	virtual QPersistentModelIndex rootIndex() const;
	virtual Id rootId() const;

	virtual bool hasRootDiagrams() const;
	virtual int childrenOfRootDiagram() const;
	virtual int childrenOfDiagram(const Id &parent) const;

	virtual void removeElement(Id const &logicalId);

	/// Returns a mapping of known editors used for current save creation to their versions.
	QMap<Id, Version> editorVersions() const;

private:
	LogicalModelAssistApi(LogicalModelAssistApi const &);  // Copying is forbidden
	LogicalModelAssistApi& operator =(LogicalModelAssistApi const &); // Assignment is forbidden also

	details::ModelsAssistApi mModelsAssistApi;
	details::LogicalModel &mLogicalModel;
	Exploser mExploser;
	EditorManagerInterface const &mEditorManager;
};

}
}
