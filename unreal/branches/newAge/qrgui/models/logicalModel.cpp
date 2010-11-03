#include "logicalModel.h"

#include "graphicalModel.h"

#include<QtCore/QDebug>

using namespace qReal;
using namespace models;
using namespace details;

LogicalModel::LogicalModel(qrRepo::RepoApi &repoApi, EditorManager const &editorManager)
	: AbstractModel(repoApi, editorManager), mGraphicalModelView(this)
{
	mRootItem = new LogicalModelItem(Id::rootId(), NULL);
	mModelItems.insert(Id::rootId(), mRootItem);
}

void LogicalModel::connectToGraphicalModel(GraphicalModel * const graphicalModel)
{
	mGraphicalModelView.setModel(graphicalModel);
}

AbstractModelItem *LogicalModel::createModelItem(Id const &id, AbstractModelItem *parentItem) const
{
	return new LogicalModelItem(id, static_cast<LogicalModelItem *>(parentItem));
}

QModelIndex LogicalModel::indexById(Id const &id) const
{
	if (mModelItems.keys().contains(id)) {
		return index(mModelItems.find(id).value());
	}
	return QModelIndex();
}

void LogicalModel::updateElements(Id const &logicalId, QString const &name)
{
	if (!mNotNeedUpdate) {
		mNotNeedUpdate = true;
		return;
	}
	mApi.setName(logicalId, name);
	emit dataChanged(indexById(logicalId), indexById(logicalId));
}

