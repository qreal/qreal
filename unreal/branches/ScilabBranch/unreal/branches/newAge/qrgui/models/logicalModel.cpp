#include "logicalModel.h"

#include "graphicalModel.h"

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
