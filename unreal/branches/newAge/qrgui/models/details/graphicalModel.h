#pragma once

#include "../../../qrrepo/repoApi.h"
#include "../../editorManager/editorManager.h"
#include "modelsImplementation/graphicalModelItem.h"
#include "modelsImplementation/abstractModel.h"
#include "logicalModelView.h"

namespace qReal {

namespace models {

namespace details {

class LogicalModel;

class GraphicalModel : public modelsImplementation::AbstractModel
{
	Q_OBJECT;

public:
	GraphicalModel(qrRepo::GraphicalRepoApi *repoApi, EditorManager const &editorManager);

	void connectToLogicalModel(LogicalModel * const logicalModel);
	void updateElements(Id const &logicalId, QString const &name);
	void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, QString const &name, QPointF const &position);
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
	virtual bool dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent);

private:
	LogicalModelView mLogicalModelView;
	qrRepo::GraphicalRepoApi &mApi;
	virtual modelsImplementation::AbstractModelItem *createModelItem(Id const &id, modelsImplementation::AbstractModelItem *parentItem) const;
	void initializeElement(const Id &id, const Id &logicalId, modelsImplementation::AbstractModelItem *parentItem,
			modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position);
};

}

}

}
