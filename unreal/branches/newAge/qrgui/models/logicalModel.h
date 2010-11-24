#pragma once

#include <QMimeData>
#include <QDebug>

#include "../../qrrepo/repoApi.h"
#include "../editorManager/editorManager.h"
#include "details/logicalModelItem.h"
#include "details/abstractModel.h"
#include "graphicalModelView.h"

namespace qReal {

	namespace models {

		class GraphicalModel;

		class LogicalModel : public details::AbstractModel
		{
			Q_OBJECT;
		public:
			LogicalModel(qrRepo::LogicalRepoApi *repoApi, EditorManager const &editorManager);

			void connectToGraphicalModel(GraphicalModel * const graphicalModel);
			void updateElements(Id const &logicalId, QString const &name);
			QModelIndex indexById(Id const &id) const;
			virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
			virtual bool dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent);
			QString pathToItem(details::AbstractModelItem const * const item) const;
			void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, QString const &name, QPointF const &position);
			void initializeElement(const Id &id, details::AbstractModelItem *parentItem,
					details::AbstractModelItem *item, const QString &name, const QPointF &position);
			virtual QVariant data(const QModelIndex &index, int role) const;
			virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
			virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
		private:
			GraphicalModelView mGraphicalModelView;
			qrRepo::LogicalRepoApi &mApi;
			virtual details::AbstractModelItem *createModelItem(Id const &id, details::AbstractModelItem *parentItem) const;
		};

	}

}
