#pragma once

#include "details/graphicalModel.h"
#include "details/logicalModel.h"

namespace qReal {

namespace models {

class Models
{
public:
	explicit Models(QString const &workingCopy, EditorManager const &editorManager);
	~Models();

	QAbstractItemModel* graphicalModel() const;
	QAbstractItemModel* logicalModel() const;
private:
	models::details::GraphicalModel *mGraphicalModel;
	models::details::LogicalModel *mLogicalModel;
	qrRepo::RepoControlInterface *mRepoApi;
};

}

}
