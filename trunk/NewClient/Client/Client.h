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
			IdTypeList children(const IdType &id) const;
			IdTypeList parents(const IdType &id) const;
			void addParent(const IdType &id, const IdType &parent);
			void addChild(const IdType &id, const IdType &child);
			void removeParent(const IdType &id, const IdType &parent);
			void removeChild(const IdType &id, const IdType &child);
			void setProperty(const IdType &id, const PropertyName &name, const QVariant &value);
			QVariant property(const IdType &id, const PropertyName &name) const;
			void removeProperty(const IdType &id, const PropertyName &name);
			void svnUpdate();
			void svnCommit();

		private:
			void loadFromDisk();
			void saveToDisk();
			QHash<IdType,LogicObject*> mObjects;
		};

	}

}
