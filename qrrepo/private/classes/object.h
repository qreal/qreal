#pragma once

#include "../../../qrkernel/ids.h"

#include <QMap>
#include <QVariant>
#include <QString>

namespace qrRepo {

	namespace details {

		class Object
		{
		public:
			explicit Object(const qReal::Id &id);

			/// replacing property values that contains input value with new value
			/// @param value - input value that should be contained by any property of element
			/// @param newValue - string representation of value with what property values should be replaced
			void replaceProperties(QString const value, QString newValue);

			Object(const qReal::Id &id, const qReal::Id &parent);
			Object(const qReal::Id &id, const qReal::Id &parent, const qReal::Id &logicalId);
			Object *clone(QHash<qReal::Id, Object*> &objHash) const;
			Object *clone(const qReal::Id &parent, QHash<qReal::Id, Object*> &objHash) const;
			void setParent(const qReal::Id &parent);
			void removeParent();
			void addChild(const qReal::Id &child);
			void removeChild(const qReal::Id &child);
			void copyPropertiesFrom(const Object &src);
			qReal::IdList children() const;
			qReal::Id parent() const;
			QVariant property(const QString &name) const;
			bool hasProperty(const QString &name, bool sensitivity = false) const;
			void setProperty(const QString &name, const QVariant &value);
			void removeProperty(const QString &name);
			qReal::Id id() const;
			qReal::Id logicalId() const;
			QMapIterator<QString, QVariant> propertiesIterator();
			void setTemporaryRemovedLinks(QString const &direction, qReal::IdList const &listValue);
			qReal::IdList temporaryRemovedLinksAt(QString const &direction) const;
			qReal::IdList temporaryRemovedLinks() const;
			void removeTemporaryRemovedLinksAt(QString const &direction);
			void removeTemporaryRemovedLinks();

			/// Stacks item element before sibling (they should have the same parent)
			void stackBefore(qReal::Id const &element, qReal::Id const &sibling);

		private:
			const qReal::Id mId;
			qReal::Id mLogicalId;
			qReal::Id mParent;
			qReal::IdList mChildren;
			QMap<QString, QVariant> mProperties;
			QMap<QString, qReal::IdList> mTemporaryRemovedLinks;
		};

	}

}
