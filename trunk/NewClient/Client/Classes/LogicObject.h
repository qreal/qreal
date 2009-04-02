#pragma once

#include "../Service/definitions.h"

#include <QMap>
#include <QVariant>

namespace qReal {

	namespace client {

		class LogicObject
		{
		public:
			LogicObject(const IdType &id, const IdType &parent);
			void addParent(const IdType &parent);
			void removeParent(const IdType &parent);
			void addChild(const IdType &child);
			void removeChild(const IdType &child);
			IdTypeList children() const;
			IdTypeList parents() const;
			void setProperty(const PropertyName &type, const QVariant &value);
			QVariant property(const PropertyName &type) const;

		private:
			const IdType mId;
			IdTypeList mParents;
			IdTypeList mChildren;
			QMap<PropertyName,QVariant> mProperties;
		};

	}

}