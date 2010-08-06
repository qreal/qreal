#pragma once

#include "../../qrrepo/repoApi.h"
#include "../editorManager/editorManager.h"
#include "details/graphicalModelItem.h"
#include "details/abstractModel.h"
#include "logicalModelView.h"

namespace qReal {

	namespace models {

		class LogicalModel;

		class GraphicalModel : public details::AbstractModel
		{
			Q_OBJECT;
		public:
			GraphicalModel(qrRepo::RepoApi &repoApi, EditorManager const &editorManager);

			virtual QVariant data(const QModelIndex &index, int role) const;

			void connectToLogicalModel(LogicalModel * const logicalModel);
			void updateElements(Id const &logicalId, QString const &name);

		private:
			LogicalModelView mLogicalModelView;

			virtual details::AbstractModelItem *createModelItem(Id const &id, details::AbstractModelItem *parentItem) const;
		};

	}

}
