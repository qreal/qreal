#pragma once

#include "../Service/definitions.h"
#include "Classes/LogicObject.h"

#include <QVariant>
#include <QHash>

namespace qReal {

	namespace client {

		class Client
		{
		public:
			Client();
			~Client();
			IdTypeList children(IdType id) const;
			IdTypeList parents(IdType id) const;
			void addParent(IdType id, IdType parent);
			void addChild(IdType id, IdType child);
			void removeParent(IdType id, IdType parent);
			void removeChild(IdType id, IdType child);
			void setProperty(IdType id, PropertyName type, QVariant value);
			QVariant property(IdType id, PropertyName type) const;
			void svnUpdate();
			void svnCommit();

		private:
			void loadFromDisk();
			void saveToDisk();
			QHash<IdType,LogicObject*> mObjects;
		};

	}

}