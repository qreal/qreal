#pragma once

#include <qrrepo/repoApi.h>

#include "pluginManager/editorManagerInterface.h"
#include "models/details/modelsImplementation/graphicalModelItem.h"
#include "models/details/modelsImplementation/abstractModel.h"
#include "models/details/logicalModelView.h"
#include "models/graphicalModelAssistApi.h"

namespace qReal {

namespace models {
class GraphicalModelAssistApi;

namespace details {

class LogicalModel;

class GraphicalModel : public modelsImplementation::AbstractModel
{
	Q_OBJECT

public:
	GraphicalModel(qrRepo::GraphicalRepoApi *repoApi, EditorManagerInterface const &editorManagerInterface, ConstraintsManager const &constraintsManager);
	virtual ~GraphicalModel();

	/// Sets graphical assist interface for this model. Takes ownership.
	void setAssistApi(GraphicalModelAssistApi * const graphicalAssistApi);

	void connectToLogicalModel(LogicalModel * const logicalModel);
	void updateElements(Id const &logicalId, QString const &name);
	void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, QString const &name
			, QPointF const &position);
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
	qrRepo::GraphicalRepoApi const &api() const;
	qrRepo::GraphicalRepoApi &mutableApi() const;
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	QList<QPersistentModelIndex> indexesWithLogicalId(Id const &logicalId) const;
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const;
	GraphicalModelAssistApi &graphicalModelAssistApi() const;
	virtual void stackBefore(QModelIndex const &element, QModelIndex const &sibling);

signals:
	void nameChanged(Id const &id);

private:
	LogicalModelView mLogicalModelView;
	qrRepo::GraphicalRepoApi &mApi;
	GraphicalModelAssistApi *mGraphicalAssistApi;  // Has ownership.

	virtual void init();
	void loadSubtreeFromClient(modelsImplementation::GraphicalModelItem * const parent);
	modelsImplementation::GraphicalModelItem *loadElement(modelsImplementation::GraphicalModelItem *parentItem
			, Id const &id);

	void setNewName(Id const &id, QString const newValue);
	virtual modelsImplementation::AbstractModelItem *createModelItem(Id const &id
			, modelsImplementation::AbstractModelItem *parentItem) const;
	void initializeElement(const Id &id, const Id &logicalId, modelsImplementation::AbstractModelItem *parentItem
			, modelsImplementation::AbstractModelItem *item, QString const &name, const QPointF &position);
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child);
};
}
}
}
