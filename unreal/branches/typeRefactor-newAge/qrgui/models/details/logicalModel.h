#pragma once

#include <QMimeData>
#include <QDebug>

#include "../../../qrrepo/repoApi.h"
#include "../../editorManager/editorManager.h"
#include "modelsImplementation/logicalModelItem.h"
#include "modelsImplementation/abstractModel.h"
#include "graphicalModelView.h"
#include "../logicalModelAssistApi.h"

namespace qReal {

	namespace models {
		class LogicalModelAssistApi;

		namespace details {

			class GraphicalModel;

			class LogicalModel : public modelsImplementation::AbstractModel
			{
				Q_OBJECT;

			public:
				LogicalModel(qrRepo::LogicalRepoApi *repoApi, EditorManager const &editorManager);
				virtual ~LogicalModel();

				void connectToGraphicalModel(GraphicalModel * const graphicalModel);
				void updateElements(Id const &logicalId, QString const &name);
				virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
				void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, NewType const &type, QString const &name, QPointF const &position);
				virtual QVariant data(const QModelIndex &index, int role) const;
				virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
				virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
				qrRepo::LogicalRepoApi const &api() const;
				qrRepo::LogicalRepoApi &mutableApi() const;
				virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
				virtual ModelsAssistApi* modelAssistApi() const;
				LogicalModelAssistApi &logicalModelAssistApi() const;

		private:
				GraphicalModelView mGraphicalModelView;
				qrRepo::LogicalRepoApi &mApi;
				LogicalModelAssistApi *mLogicalAssistApi;

				virtual void init();
				void loadSubtreeFromClient(modelsImplementation::LogicalModelItem * const parent);
				modelsImplementation::LogicalModelItem *loadElement(modelsImplementation::LogicalModelItem *parentItem, Id const &id);
				void checkProperties(Id const &id);

				virtual modelsImplementation::AbstractModelItem *createModelItem(Id const &id, modelsImplementation::AbstractModelItem *parentItem) const;
				void initializeElement(const Id &id, const NewType &type, modelsImplementation::AbstractModelItem *parentItem,
						modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position);
				QString pathToItem(modelsImplementation::AbstractModelItem const * const item) const;
				virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root, details::modelsImplementation::AbstractModelItem *child);
			};
		}

	}

}
