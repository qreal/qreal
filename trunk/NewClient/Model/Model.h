#pragma once

#include "../Client/Client.h"
#include "../Service/definitions.h"
#include "Classes/ModelTreeItem.h"

#include <QAbstractItemModel>
#include <QVariant>
#include <QStringList>
#include <QMimeData>
#include <QModelIndexList>
#include <QPointF>

namespace qReal {

	namespace model {

		class Model : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			Model();
			~Model();
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
			void test();

		private:
			client::Client *mClient;
			QMultiHash<IdType,ModelTreeItem*> treeItems;
			ModelTreeItem *rootItem;

			PropertyName pathToItem(ModelTreeItem *item) const;
			void removeConfigurationInClient(ModelTreeItem *item);
			QModelIndex index(ModelTreeItem *item);
			void removeModelItems(ModelTreeItem *root);
			bool addElementToModel(ModelTreeItem *parentItem, const IdType &id, 
				const PropertyName &pathToItem, const QString &name, const QPointF &position, Qt::DropAction action);
		};

	}

}
