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
			Qt::ItemFlags flags(const QModelIndex &index) const;
			QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
			QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			int rowCount(const QModelIndex &parent = QModelIndex()) const;
			int columnCount(const QModelIndex &parent = QModelIndex()) const;
			bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole); 
			bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());  
			QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const; 
			QModelIndex parent(const QModelIndex &index) const;

		private:
			client::Client *mClient;
			void removeModelItems(ModelTreeItem *root);
		};

	}

}
