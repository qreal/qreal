#pragma once

#include "../../kernel/definitions.h"

#include <QMap>
#include <QVariant>

namespace qReal {

	namespace client {

		namespace details {

			class LogicObject
			{
			public:
				explicit LogicObject(const Id &id);
				LogicObject(const Id &id, const Id &parent);
				void addParent(const Id &parent);
				void removeParent(const Id &parent);
				void addChild(const Id &child);
				void removeChild(const Id &child);
				IdList children() const;
				IdList parents() const;
				void setProperty(const QString &name, const QVariant &value);
				QVariant property(const QString &name) const;
				bool hasProperty(const QString &name) const;
				void removeProperty(const QString &name);
				Id id() const;
				QMapIterator<QString, QVariant> propertiesIterator();

			private:
				const Id mId;
				IdList mParents;
				IdList mChildren;
				QMap<QString, QVariant> mProperties;
			};

		}

	}

}
