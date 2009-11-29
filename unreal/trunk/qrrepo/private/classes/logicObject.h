#pragma once

#include "../../../qrgui/kernel/definitions.h"

#include <QMap>
#include <QVariant>

namespace qrRepo {

	namespace details {

		class LogicObject
		{
		public:
			explicit LogicObject(const qReal::Id &id);
			LogicObject(const qReal::Id &id, const qReal::Id &parent);
			void addParent(const qReal::Id &parent);
			void removeParent(const qReal::Id &parent);
			void addChild(const qReal::Id &child);
			void removeChild(const qReal::Id &child);
			qReal::IdList children() const;
			qReal::IdList parents() const;
			void setProperty(const QString &name, const QVariant &value);
			QVariant property(const QString &name) const;
			bool hasProperty(const QString &name) const;
			void removeProperty(const QString &name);
			qReal::Id id() const;
			QMapIterator<QString, QVariant> propertiesIterator();

		private:
			const qReal::Id mId;
			qReal::IdList mParents;
			qReal::IdList mChildren;
			QMap<QString, QVariant> mProperties;
		};

	}

}
