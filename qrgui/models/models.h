#pragma once

#include "models/details/graphicalModel.h"
#include "models/details/logicalModel.h"
#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"
#include "models/exploser.h"

namespace qReal {
namespace models {

class QRGUI_MODELS_EXPORT Models
{
public:
	Models(QString const &workingCopy, EditorManagerInterface &editorManager);
	~Models();

	QAbstractItemModel *graphicalModel() const;
	QAbstractItemModel *logicalModel() const;

	GraphicalModelAssistApi &graphicalModelAssistApi() const;
	LogicalModelAssistApi &logicalModelAssistApi() const;

	qrRepo::RepoControlInterface &repoControlApi() const;

	qrRepo::LogicalRepoApi const &logicalRepoApi() const;
	qrRepo::LogicalRepoApi &mutableLogicalRepoApi() const;

	qrRepo::GraphicalRepoApi const &graphicalRepoApi() const;

	Exploser &exploser() const;

	void reinit();

private:
	models::details::GraphicalModel *mGraphicalModel;
	models::details::GraphicalPartModel *mGraphicalPartModel;
	models::details::LogicalModel *mLogicalModel;
	qrRepo::RepoControlInterface *mRepoApi;
	Exploser *mExploser;
};

}
}
