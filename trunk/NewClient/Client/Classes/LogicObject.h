#pragma once

#include "definitions.h"

#include <QMap>
#include <QVariant>

namespace qReal {

	namespace client {

		class LogicObject
		{
		public:
			LogicObject(IdType id, IdType parent);
			void addParent(IdType parent);
			void removeParent(IdType parent);
			void addChild(IdType child);
			void removeChild(IdType child);
			IdTypeList children();
			IdTypeList parents();
			void setProperty(PropertyType type, QVariant value);
			QVariant property(PropertyType type);

		private:
			const IdType mId;
			IdTypeList mParents;
			IdTypeList mChildren;
			QMap<PropertyType,QVariant> mProperties;
		};

	}

}