#include "models.h"

using namespace qReal;
using namespace models;

Models::Models(QString const &workingCopy, EditorManager const &editorManager)
{
	qrRepo::RepoApi *repoApi = new qrRepo::RepoApi(workingCopy);
	mGraphicalModel = new models::details::GraphicalModel(repoApi, editorManager);
	mLogicalModel = new models::details::LogicalModel(repoApi, editorManager, false);
	mLibraryModel = new models::details::LogicalModel(repoApi, editorManager, true);
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

QAbstractItemModel* Models::libraryModel() const
{
	return mLibraryModel;
}


GraphicalModelAssistApi &Models::graphicalModelAssistApi() const
{
	return mGraphicalModel->graphicalModelAssistApi();
}

LogicalModelAssistApi &Models::logicalModelAssistApi() const
{
	return mLogicalModel->logicalModelAssistApi();
}

LogicalModelAssistApi &Models::libraryModelAssistApi() const
{
	 return mLibraryModel->logicalModelAssistApi();
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

qrRepo::GraphicalRepoApi &Models::mutableGraphicalRepoApi() const
{
	return mGraphicalModel->mutableApi();
}

//Be aware that in current realisation metohd init() used in reinit() methods
//taks only default client into account - the one that handles default project savefile
void Models::reinit()
{
	mLogicalModel->reinit();
	mGraphicalModel->reinit();

	//efimefim
	mLibraryModel->reinit();
}
