#pragma once

#include "../../../qrrepo/repoApi.h"
#include "../../editorManager/editorManager.h"
#include "modelsImplementation/graphicalModelItem.h"
#include "modelsImplementation/abstractModel.h"
#include "logicalModelView.h"
#include "../graphicalModelAssistApi.h"

namespace qReal {

	namespace models {
		class GraphicalModelAssistApi;

		namespace details {

			class LogicalModel;

			class GraphicalModel : public modelsImplementation::AbstractModel
			{
				Q_OBJECT;

			public:
				GraphicalModel(qrRepo::GraphicalRepoApi *repoApi, EditorManager const &editorManager);
				virtual ~GraphicalModel();

				void connectToLogicalModel(LogicalModel * const logicalModel);
				void updateElements(Id const &logicalId, QString const &name);
				void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, QString const &name, QPointF const &position);
				virtual QVariant data(const QModelIndex &index, int role) const;
				virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
				virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
				virtual bool dropMimeData(QMimeData const *data, Qt::DropAction action, int row, int column, QModelIndex const &parent);
				qrRepo::GraphicalRepoApi const &api() const;
				qrRepo::GraphicalRepoApi &mutableApi() const;
				GraphicalModelAssistApi &graphicalModelAssistApi() const;
				virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
				QList<QPersistentModelIndex> indexesWithLogicalId(Id const &logicalId) const;

			private:
				LogicalModelView mLogicalModelView;
				qrRepo::GraphicalRepoApi &mApi;
				GraphicalModelAssistApi *mGraphicalAssistApi;

				virtual modelsImplementation::AbstractModelItem *createModelItem(Id const &id, modelsImplementation::AbstractModelItem *parentItem) const;
				void initializeElement(const Id &id, const Id &logicalId, modelsImplementation::AbstractModelItem *parentItem,
				modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position);
				void removeModelItems(details::modelsImplementation::AbstractModelItem * const root);
			};
		}
	}
}
