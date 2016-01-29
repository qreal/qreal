/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "models.h"

#include "exploser.h"

using namespace qReal;
using namespace models;

Models::Models(const QString &workingCopy, const EditorManagerInterface &editorManager)
{
	qrRepo::RepoApi *repoApi = new qrRepo::RepoApi(workingCopy);
	mGraphicalModel = new models::details::GraphicalModel(repoApi, editorManager);
	mGraphicalPartModel = new models::details::GraphicalPartModel(*repoApi, *mGraphicalModel);

	GraphicalModelAssistApi * const graphicalAssistApi
			= new GraphicalModelAssistApi(*mGraphicalModel, *mGraphicalPartModel, editorManager);

	mGraphicalModel->setAssistApi(graphicalAssistApi);
	mGraphicalModel->reinit();

	QObject::connect(mGraphicalModel, SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int))
			, mGraphicalPartModel, SLOT(rowsAboutToBeRemovedInGraphicalModel(QModelIndex, int, int)));

	mLogicalModel = new models::details::LogicalModel(repoApi, editorManager);
	mExploser.reset(new Exploser(logicalModelAssistApi()));
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

const qrRepo::LogicalRepoApi &Models::logicalRepoApi() const
{
	return mLogicalModel->api();
}

qrRepo::LogicalRepoApi &Models::mutableLogicalRepoApi() const
{
	return mLogicalModel->mutableApi();
}

const qrRepo::GraphicalRepoApi &Models::graphicalRepoApi() const
{
	return mGraphicalModel->api();
}

Exploser &Models::exploser() const
{
	return *mExploser;
}

void Models::reinit()
{
	mLogicalModel->reinit();
	mGraphicalModel->reinit();
	mGraphicalPartModel->reinit();
}
