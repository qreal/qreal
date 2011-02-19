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
				void addElementToModel(Id const &parent, Id const &id,Id const &logicalId, NewType const &type, QString const &name, QPointF const &position);
				virtual QVariant data(const QModelIndex &index, int role) const;
				virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
				virtual void changeParent(QModelIndex const &element, QModelIndex const &parent, QPointF const &position);
				qrRepo::GraphicalRepoApi const &api() const;
				qrRepo::GraphicalRepoApi &mutableApi() const;
				virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
				QList<QPersistentModelIndex> indexesWithLogicalId(Id const &logicalId) const;
				virtual ModelsAssistApi* modelAssistApi() const;
				GraphicalModelAssistApi &graphicalModelAssistApi() const;

			signals:
				void nameChanged(Id const &id);

			private:
				LogicalModelView mLogicalModelView;
				qrRepo::GraphicalRepoApi &mApi;
				GraphicalModelAssistApi *mGraphicalAssistApi;

				virtual void init();
				void loadSubtreeFromClient(modelsImplementation::GraphicalModelItem * const parent);
				modelsImplementation::GraphicalModelItem *loadElement(modelsImplementation::GraphicalModelItem *parentItem, Id const &id);

				virtual modelsImplementation::AbstractModelItem *createModelItem(Id const &id, modelsImplementation::AbstractModelItem *parentItem) const;
				void initializeElement(const Id &id, const Id &logicalId, const NewType &type, modelsImplementation::AbstractModelItem *parentItem,
				modelsImplementation::AbstractModelItem *item, const QString &name, const QPointF &position);
				virtual void removeModelItemFromApi(details::modelsImplementation::AbstractModelItem *const root, details::modelsImplementation::AbstractModelItem *child);
			};
		}
	}
}
