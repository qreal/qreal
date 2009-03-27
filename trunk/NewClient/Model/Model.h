#pragma once

#include "../Client/Client.h"

#include <QAbstractItemModel>
#include <QVariant>

namespace qReal {

	namespace model {

		class Model : public QAbstractItemModel
		{
		public:
			Model();
			~Model();
			Qt::ItemFlags flags(const QModelIndex &index) const;
			QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

		private:
			client::Client *mClient;
		};

	}

}
