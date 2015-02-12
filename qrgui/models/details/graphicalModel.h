#pragma once

#include <qrrepo/repoApi.h>

#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "modelsImplementation/graphicalModelItem.h"
#include "modelsImplementation/abstractModel.h"
#include "logicalModelView.h"
#include "qrgui/models/graphicalModelAssistApi.h"

namespace qReal {

namespace models {
class GraphicalModelAssistApi;

namespace details {

class LogicalModel;

class QRGUI_MODELS_EXPORT GraphicalModel : public modelsImplementation::AbstractModel
{
	Q_OBJECT

public:
	GraphicalModel(qrRepo::GraphicalRepoApi *repoApi
			, const EditorManagerInterface &editorManagerInterface);

	virtual ~GraphicalModel();

	/// Sets graphical assist interface for this model. Takes ownership.
	void setAssistApi(GraphicalModelAssistApi * const graphicalAssistApi);

	void connectToLogicalModel(LogicalModel * const logicalModel);
	void updateElements(const Id &logicalId, const QString &name);
	void addElementToModel(const Id &parent, const Id &id,const Id &logicalId, const QString &name
			, const QPointF &position);
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual void changeParent(const QModelIndex &element, const QModelIndex &parent, const QPointF &position);
	const qrRepo::GraphicalRepoApi &api() const;
	qrRepo::GraphicalRepoApi &mutableApi() const;
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	QList<QPersistentModelIndex> indexesWithLogicalId(const Id &logicalId) const;
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const;
	GraphicalModelAssistApi &graphicalModelAssistApi() const;
	virtual void stackBefore(const QModelIndex &element, const QModelIndex &sibling);

signals:
	void nameChanged(const Id &id);

private:
	LogicalModelView mLogicalModelView;
	qrRepo::GraphicalRepoApi &mApi;
	GraphicalModelAssistApi *mGraphicalAssistApi;  // Has ownership.

	virtual void init();
	void loadSubtreeFromClient(modelsImplementation::GraphicalModelItem * const parent);
	modelsImplementation::GraphicalModelItem *loadElement(modelsImplementation::GraphicalModelItem *parentItem
			, const Id &id);

	void setNewName(const Id &id, const QString newValue);
	virtual modelsImplementation::AbstractModelItem *createModelItem(const Id &id
			, modelsImplementation::AbstractModelItem *parentItem) const;
	void initializeElement(const Id &id, const Id &logicalId, modelsImplementation::AbstractModelItem *parentItem
			, modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position);
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child);
};
}
}
}
