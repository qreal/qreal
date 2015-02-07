#pragma once

#include <QtCore/QMimeData>

#include <qrrepo/repoApi.h>

#include "plugins/pluginManager/editorManagerInterface.h"
#include "models/details/modelsImplementation/logicalModelItem.h"
#include "models/details/modelsImplementation/abstractModel.h"
#include "models/details/graphicalModelView.h"
#include "models/logicalModelAssistApi.h"

namespace qReal {

namespace models {
class LogicalModelAssistApi;

namespace details {

class GraphicalModel;

class QRGUI_MODELS_EXPORT LogicalModel : public modelsImplementation::AbstractModel
{
	Q_OBJECT

public:
	LogicalModel(qrRepo::LogicalRepoApi *repoApi, const EditorManagerInterface &editorManagerInterface);
	virtual ~LogicalModel();

	void connectToGraphicalModel(GraphicalModel * const graphicalModel);
	void updateElements(const Id &logicalId, const QString &name);
	virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
	void addElementToModel(const Id &parent, const Id &id,const Id &logicalId, const QString &name
			, const QPointF &position);
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual void changeParent(const QModelIndex &element, const QModelIndex &parent, const QPointF &position);
	void changeParent(const Id &parentId, const Id &childId);
	const qrRepo::LogicalRepoApi &api() const;
	qrRepo::LogicalRepoApi &mutableApi() const;
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const;
	LogicalModelAssistApi &logicalModelAssistApi() const;
	virtual void stackBefore(const QModelIndex &element, const QModelIndex &sibling);

private:
	virtual void init();
	void loadSubtreeFromClient(modelsImplementation::LogicalModelItem * const parent);
	modelsImplementation::LogicalModelItem *loadElement(modelsImplementation::LogicalModelItem *parentItem
			, const Id &id);
	void addInsufficientProperties(const Id &id, const QString &name = QString());

	virtual modelsImplementation::AbstractModelItem *createModelItem(const Id &id
			, modelsImplementation::AbstractModelItem *parentItem) const;
	void initializeElement(const Id &id, modelsImplementation::AbstractModelItem *parentItem
			, modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position);
	QString pathToItem(const modelsImplementation::AbstractModelItem * const item) const;
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child);

	GraphicalModelView mGraphicalModelView;
	qrRepo::LogicalRepoApi &mApi;
	LogicalModelAssistApi *mLogicalAssistApi;
};
}

}

}
