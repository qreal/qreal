#pragma once

#include <qrkernel/ids.h>

#include "models/modelsDeclSpec.h"
#include "models/details/logicalModel.h"
#include "models/details/modelsAssistApi.h"
#include "plugins/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"

namespace qReal {

class EditorManager;
class EditorManagerInterface;

namespace models {

class QRGUI_MODELS_EXPORT LogicalModelAssistApi : public qReal::LogicalModelAssistInterface
{
public:
	LogicalModelAssistApi(details::LogicalModel &logicalModel, EditorManagerInterface const &editorManagerInterface);
	virtual ~LogicalModelAssistApi();

	EditorManagerInterface const &editorManagerInterface() const;

	qrRepo::LogicalRepoApi const &logicalRepoApi() const override;
	qrRepo::LogicalRepoApi &mutableLogicalRepoApi() override;
	Id createElement(Id const &parent, Id const &type) override;
	Id createElement(Id const &parent, Id const &id, bool isFromLogicalModel, QString const &name
			, QPointF const &position, Id const &preferedLogicalId = Id()) override;

	IdList children(Id const &element) const override;
	void changeParent(Id const &element, Id const &parent, QPointF const &position = QPointF()) override;

	void addExplosion(Id const &source, Id const &destination) override;
	void removeExplosion(Id const &source, Id const &destination) override;

	void setPropertyByRoleName(Id const &elem, QVariant const &newValue, QString const &roleName) override;
	QVariant propertyByRoleName(Id const &elem, QString const &roleName) const override;

	bool isLogicalId(Id const &id) const override;

	void removeReferencesTo(Id const &id) override;
	void removeReferencesFrom(Id const &id) override;
	void removeReference(Id const &id, Id const &reference) override;

	void setName(Id const &elem, QString const &newValue) override;
	QString name(Id const &elem) const override;

	void setTo(Id const &elem, Id const &newValue) override;
	Id to(Id const &elem) const override;

	void setFrom(Id const &elem, Id const &newValue) override;
	Id from(Id const &elem) const override;

	QModelIndex indexById(Id const &id) const override;
	Id idByIndex(QModelIndex const &index) const override;
	QPersistentModelIndex rootIndex() const override;
	Id rootId() const override;

	bool hasRootDiagrams() const override;
	int childrenOfRootDiagram() const override;
	int childrenOfDiagram(const Id &parent) const override;

	void removeElement(Id const &logicalId) override;

	/// Returns a mapping of known editors used for current save creation to their versions.
	QMap<Id, Version> editorVersions() const;

private:
	LogicalModelAssistApi(LogicalModelAssistApi const &);  // Copying is forbidden
	LogicalModelAssistApi& operator =(LogicalModelAssistApi const &); // Assignment is forbidden also

	details::ModelsAssistApi mModelsAssistApi;
	details::LogicalModel &mLogicalModel;
	EditorManagerInterface const &mEditorManager;
};

}
}
