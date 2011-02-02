#pragma once

#include "../../../qrgui/kernel/roles.h"
#include "../../../qrgui/kernel/NewType.h"
#include <QMap>
#include <QVariant>

namespace qrRepo {

	namespace details {

		class LogicObject
		{
		public:
                        explicit LogicObject(const qReal::NewType &type);
                        LogicObject(const qReal::NewType &id, const qReal::NewType &parent);
                        void addParent(const qReal::NewType &parent);
                        void removeParent(const qReal::NewType &parent);
                        void addChild(const qReal::NewType &child);
                        void removeChild(const qReal::NewType &child);
                        qReal::TypeList children() const;
                        qReal::TypeList parents() const;
			void setProperty(const QString &name, const QVariant &value);
			QVariant property(const QString &name) const;
			bool hasProperty(const QString &name) const;
			void removeProperty(const QString &name);
                        qReal::NewType type() const;
			QMapIterator<QString, QVariant> propertiesIterator();

		private:
                        const qReal::NewType mType;
                        qReal::TypeList mParents;
                        qReal::TypeList mChildren;
			QMap<QString, QVariant> mProperties;
		};

	}

}
