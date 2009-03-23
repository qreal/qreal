#pragma once

#include "../Client/Client.h"

#include <QAbstractItemModel>

namespace qReal {

	namespace model {

		class Model : public QAbstractItemModel
		{
		public:
			Model();
			~Model();

		private:
			client::Client *mClient;
		};

	}

}
