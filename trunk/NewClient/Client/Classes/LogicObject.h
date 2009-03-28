#pragma once

#include "../Service/definitions.h"

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
			IdTypeList children() const;
			IdTypeList parents() const;
			void setProperty(PropertyName type, QVariant value);
			QVariant property(PropertyName type) const;

		private:
			const IdType mId;
			IdTypeList mParents;
			IdTypeList mChildren;
			QMap<PropertyName,QVariant> mProperties;
		};

	}

}