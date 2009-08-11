#pragma once

#include "../../kernel/definitions.h"

#include <QMap>
#include <QVariant>

namespace qReal {

	namespace client {

		class LogicObject
		{
		public:
			explicit LogicObject(const IdType &id);
			LogicObject(const IdType &id, const IdType &parent);
			void addParent(const IdType &parent);
			void removeParent(const IdType &parent);
			void addChild(const IdType &child);
			void removeChild(const IdType &child);
			IdTypeList children() const;
			IdTypeList parents() const;
			void setProperty(const PropertyName &name, const QVariant &value);
			QVariant property(const PropertyName &name) const;
			bool hasProperty(const PropertyName &name) const;
			void removeProperty(const PropertyName &name);
			IdType id() const;
			QMapIterator<PropertyName, QVariant> propertiesIterator();

		private:
			const IdType mId;
			IdTypeList mParents;
			IdTypeList mChildren;
			QMap<PropertyName,QVariant> mProperties;
		};

	}

}
