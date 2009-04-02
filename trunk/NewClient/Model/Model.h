#pragma once

#include "../Client/Client.h"
#include "../Service/definitions.h"
#include "Classes/ModelTreeItem.h"

#include <QAbstractItemModel>
#include <QVariant>

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

		private:
			client::Client *mClient;
			void removeModelItems(ModelTreeItem *root);
			QMultiHash<IdType,ModelTreeItem*> treeItems;
			ModelTreeItem *rootItem;
		};

	}

}
