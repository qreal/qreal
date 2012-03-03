#include "models.h"

using namespace qReal;
using namespace models;

Models::Models(QString const &workingCopy, InterpreterEditorManager const &editorManager)
{
	qrRepo::RepoApi *repoApi = new qrRepo::RepoApi(workingCopy);
	mGraphicalModel = new models::details::GraphicalModel(repoApi, editorManager);
	mLogicalModel = new models::details::LogicalModel(repoApi, editorManager);
	mRepoApi = repoApi;

	mLogicalModel->connectToGraphicalModel(mGraphicalModel);
	mGraphicalModel->connectToLogicalModel(mLogicalModel);
}

Models::~Models()
{
	delete mGraphicalModel;
	delete mLogicalModel;
	delete mRepoApi;
}

QAbstractItemModel* Models::graphicalModel() const
{
	return mGraphicalModel;
}

QAbstractItemModel* Models::logicalModel() const
{
	return mLogicalModel;
}

GraphicalModelAssistApi &Models::graphicalModelAssistApi() const
{
	return mGraphicalModel->graphicalModelAssistApi();
}

LogicalModelAssistApi &Models::logicalModelAssistApi() const
{
	return mLogicalModel->logicalModelAssistApi();
}

qrRepo::RepoControlInterface &Models::repoControlApi() const
{
	return *mRepoApi;
}

qrRepo::LogicalRepoApi const &Models::logicalRepoApi() const
{
	return mLogicalModel->api();
}

qrRepo::LogicalRepoApi &Models::mutableLogicalRepoApi() const
{
	return mLogicalModel->mutableApi();
}

qrRepo::GraphicalRepoApi const &Models::graphicalRepoApi() const
{
	return mGraphicalModel->api();
}

void Models::reinit()
{
	mLogicalModel->reinit();
	mGraphicalModel->reinit();
}
