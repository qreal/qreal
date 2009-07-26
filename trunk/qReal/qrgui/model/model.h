#pragma once

#include "../client/client.h"
#include "../kernel/definitions.h"
#include "classes/modelTreeItem.h"

#include <QAbstractItemModel>
#include <QVariant>
#include <QStringList>
#include <QMimeData>
#include <QModelIndexList>
#include <QPointF>
#include <QPersistentModelIndex>

namespace qReal {

	namespace model {

		class Model : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			Model();
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

		protected:
			QMultiHash<IdType,ModelTreeItem*> treeItems;
			QModelIndex index(ModelTreeItem *item);
			ModelTreeItem* addElementToModel(ModelTreeItem *parentItem, const IdType &id,
				const PropertyName &oldPathToItem, const QString &name, const QPointF &position, Qt::DropAction action);

		private:
			client::Client *mClient;
			ModelTreeItem *rootItem;

			Model(Model const &);  // Копировать модель нельзя
			Model& operator =(Model const &);  // Присваивать тоже

			PropertyName pathToItem(ModelTreeItem const *item) const;
			void removeConfigurationInClient(ModelTreeItem *item);
			void removeModelItems(ModelTreeItem *root);
			void loadSubtreeFromClient(ModelTreeItem * const parent);
			ModelTreeItem *loadElement(ModelTreeItem *parentItem, const IdType &id);

			PropertyName positionPropertyName(ModelTreeItem const *item) const;
			PropertyName configurationPropertyName(ModelTreeItem const *item) const;
		};

	}

}
