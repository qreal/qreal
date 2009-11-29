#pragma once

#include <QAbstractItemModel>
#include <QVariant>
#include <QStringList>
#include <QMimeData>
#include <QModelIndexList>
#include <QPointF>
#include <QPersistentModelIndex>

#include "../../qrrepo/repoApi.h"
#include "../kernel/definitions.h"
#include "classes/modelTreeItem.h"
#include "../mainwindow/editormanager.h"

namespace qReal {

	namespace model {

		class Model : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			explicit Model(EditorManager const &editorManager);
			~Model();
			QPersistentModelIndex rootIndex();
			virtual Qt::ItemFlags flags(const QModelIndex &index) const;
			virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
			virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
			virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
			virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
			virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
			virtual QModelIndex parent(const QModelIndex &index) const;
			virtual Qt::DropActions supportedDropActions() const;
			virtual QStringList mimeTypes() const;
			virtual QMimeData* mimeData(const QModelIndexList &indexes) const;
			virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
			client::RepoApi const &api() const;
		public slots:
			void exterminate();

		protected:
			QMultiHash<Id, ModelTreeItem*> treeItems;
			QModelIndex index(ModelTreeItem *item);
			ModelTreeItem* addElementToModel(ModelTreeItem *parentItem, const Id &id,
				const QString &oldPathToItem, const QString &name, const QPointF &position, Qt::DropAction action);

		private:
			client::RepoApi mApi;
			ModelTreeItem *rootItem;
			EditorManager const &mEditorManager;

			Model(Model const &);  // Копировать модель нельзя
			Model& operator =(Model const &);  // Присваивать тоже

			QString pathToItem(ModelTreeItem const *item) const;
			void removeConfigurationInClient(ModelTreeItem *item);
			void removeModelItems(ModelTreeItem *root);
			void loadSubtreeFromClient(ModelTreeItem * const parent);
			ModelTreeItem *loadElement(ModelTreeItem *parentItem, const Id &id);

			QString positionPropertyName(ModelTreeItem const *item) const;
			QString configurationPropertyName(ModelTreeItem const *item) const;
			QString findPropertyName(Id const &id, int const role) const;
			bool isDiagram(const Id &id);

			void init();
			void cleanupTree(ModelTreeItem *root);
		};

	}

}
