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
			GraphicalModel(qrRepo::GraphicalRepoApi *repoApi, EditorManager const &editorManager);

			void connectToLogicalModel(LogicalModel * const logicalModel);
			void updateElements(Id const &logicalId, QString const &name);
			void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, QString const &name, QPointF const &position);
			void initializeElement(const Id &id, const Id &logicalId, details::AbstractModelItem *parentItem,
					details::AbstractModelItem *item, const QString &name, const QPointF &position);
			virtual QVariant data(const QModelIndex &index, int role) const;
			virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
			virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
			virtual bool dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent);
		private:
			LogicalModelView mLogicalModelView;
			qrRepo::GraphicalRepoApi &mApi;
			virtual details::AbstractModelItem *createModelItem(Id const &id, details::AbstractModelItem *parentItem) const;
		};

	}

}
