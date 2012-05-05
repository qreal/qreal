#pragma once

#include "details/graphicalModel.h"
#include "details/logicalModel.h"
#include "graphicalModelAssistApi.h"
#include "logicalModelAssistApi.h"

namespace qReal {

namespace models {

class Models
{
public:
	explicit Models(QString const &workingCopy, EditorManager const &editorManager);
	~Models();

	QAbstractItemModel* graphicalModel() const;
	QAbstractItemModel* logicalModel() const;
    QAbstractItemModel* libraryModel() const;

	GraphicalModelAssistApi &graphicalModelAssistApi() const;
	LogicalModelAssistApi &logicalModelAssistApi() const;

	qrRepo::RepoControlInterface &repoControlApi() const;

	qrRepo::LogicalRepoApi const &logicalRepoApi() const;
	qrRepo::LogicalRepoApi &mutableLogicalRepoApi() const;

	qrRepo::GraphicalRepoApi const &graphicalRepoApi() const;
    qrRepo::GraphicalRepoApi &mutableGraphicalRepoApi() const;

	void reinit();

private:
	models::details::GraphicalModel *mGraphicalModel;
	models::details::LogicalModel *mLogicalModel;
    models::details::LogicalModel *mLibraryModel;
	qrRepo::RepoControlInterface *mRepoApi;
};

}

}
