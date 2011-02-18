#pragma once

#include "../../../qrgui/kernel/roles.h"
#include "../../../qrgui/kernel/newType.h"

#include <QMap>
#include <QVariant>

namespace qrRepo {

	namespace details {

		class Object
		{
		public:
			explicit Object(const qReal::Id &id, const qReal::NewType &type);
			Object(const qReal::Id &id, const qReal::Id &parent, const qReal::NewType &type);
			Object(const qReal::Id &id, const qReal::Id &parent, const qReal::Id &logicalId);
			void setParent(const qReal::Id &parent);
			void removeParent();
			void addChild(const qReal::Id &child);
			void removeChild(const qReal::Id &child);
			qReal::IdList children() const;
			qReal::Id parent() const;
			void setProperty(const QString &name, const QVariant &value);
			QVariant property(const QString &name) const;
			bool hasProperty(const QString &name) const;
			void removeProperty(const QString &name);
			qReal::Id id() const;
			qReal::Id logicalId() const;
			qReal::NewType type() const;
			void setType(qReal::NewType const &type);
			QMapIterator<QString, QVariant> propertiesIterator();
			void setTemporaryRemovedLinks(QString const &direction, qReal::IdList const &listValue);
			qReal::IdList temporaryRemovedLinksAt(QString const &direction) const;
			qReal::IdList temporaryRemovedLinks() const;
			void removeTemporaryRemovedLinksAt(QString const &direction);
			void removeTemporaryRemovedLinks();

		private:
			const qReal::Id mId;
			qReal::NewType mType;
			qReal::Id mLogicalId;
			qReal::Id mParent;
			qReal::IdList mChildren;
			QMap<QString, QVariant> mProperties;
			QMap<QString, qReal::IdList> mTemporaryRemovedLinks;
		};

	}

}
