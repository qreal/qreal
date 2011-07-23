#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

#include "kernelDeclSpec.h"

namespace qReal {

	class QRKERNEL_EXPORT Id {
	public:
		static Id loadFromString(QString const &string);
		static Id createElementId(QString const &editor, QString const &diagram, QString const &element);
		static Id rootId();

		explicit Id(QString const &editor = "", QString  const &diagram = "",
			QString  const &element = "", QString  const &id = "");
		Id(Id const &base, QString const &additional);

		QUrl toUrl() const;
		QString id() const;
		QString editor() const;
		QString diagram() const;
		QString element() const;
		QString toString() const;
		unsigned idSize() const;

		// applicable only to element's IDs. returnes type's ID
		Id type() const;

		Id sameTypeId() const;

		// cast to QVariant. not an operator, to avoid problems with autocasts
		QVariant toVariant() const;

		// default destructor and copy constuctor are OK
	private:
		QString mEditor;
		QString mDiagram;
		QString mElement;
		QString mId;

		// used only for debug
		bool checkIntegrity() const;
	};

	inline bool operator==(Id const &i1, Id const &i2)
	{
		return i1.editor() == i2.editor()
			&& i1.diagram() == i2.diagram()
			&& i1.element() == i2.element()
			&& i1.id() == i2.id();
	}

	inline bool operator!=(Id const &i1, Id const &i2)
	{
		return !(i1 == i2);
	}

	inline uint qHash(Id const &key)
	{
		return qHash(key.editor()) ^ qHash(key.diagram()) ^ qHash(key.element())
				^ qHash(key.id());
	}

	inline QDebug operator<<(QDebug dbg, Id const &id)
	{
		dbg << id.toString();
		return dbg.space();
	}

	typedef QList<Id> IdList;

	class QRKERNEL_EXPORT IdListHelper {
	public:
		static QVariant toVariant(IdList const &list);
	};

	typedef Id Metatype;
}

// qReal::Id and qReal::IdList could be used straight in QVariant
Q_DECLARE_METATYPE(qReal::Id)

Q_DECLARE_METATYPE(qReal::IdList)
