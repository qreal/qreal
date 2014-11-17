#pragma once

#include <QtCore/QMimeData>

#include <qrrepo/repoApi.h>

#include "qrgui/plugins/pluginManager/editorManagerInterface.h"
#include "qrgui/models/details/modelsImplementation/logicalModelItem.h"
#include "qrgui/models/details/modelsImplementation/abstractModel.h"
#include "qrgui/models/details/graphicalModelView.h"
#include "qrgui/models/logicalModelAssistApi.h"

namespace qReal {

namespace models {
class LogicalModelAssistApi;

namespace details {

class GraphicalModel;

class QRGUI_MODELS_EXPORT LogicalModel : public modelsImplementation::AbstractModel
{
	Q_OBJECT

public:
	LogicalModel(qrRepo::LogicalRepoApi *repoApi, EditorManagerInterface const &editorManagerInterface);
	virtual ~LogicalModel();

	void connectToGraphicalModel(GraphicalModel * const graphicalModel);
	void updateElements(Id const &logicalId, QString const &name);
	virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
	void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, QString const &name
			, QPointF const &position);
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
	void changeParent(Id const &parentId, Id const &childId);
	qrRepo::LogicalRepoApi const &api() const;
	qrRepo::LogicalRepoApi &mutableApi() const;
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	virtual qReal::details::ModelsAssistInterface* modelAssistInterface() const;
	LogicalModelAssistApi &logicalModelAssistApi() const;
	virtual void stackBefore(QModelIndex const &element, QModelIndex const &sibling);

private:
	virtual void init();
	void loadSubtreeFromClient(modelsImplementation::LogicalModelItem * const parent);
	modelsImplementation::LogicalModelItem *loadElement(modelsImplementation::LogicalModelItem *parentItem
			, Id const &id);
	void addInsufficientProperties(Id const &id, QString const &name = QString());

	virtual modelsImplementation::AbstractModelItem *createModelItem(Id const &id
			, modelsImplementation::AbstractModelItem *parentItem) const;
	void initializeElement(const Id &id, modelsImplementation::AbstractModelItem *parentItem
			, modelsImplementation::AbstractModelItem *item, QString const &name, const QPointF &position);
	QString pathToItem(modelsImplementation::AbstractModelItem const * const item) const;
	virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root
			, details::modelsImplementation::AbstractModelItem *child);

	GraphicalModelView mGraphicalModelView;
	qrRepo::LogicalRepoApi &mApi;
	LogicalModelAssistApi *mLogicalAssistApi;
};
}

}

}
