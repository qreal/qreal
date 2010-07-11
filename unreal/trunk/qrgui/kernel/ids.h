#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QMetaType>

namespace qReal {

	class Id {
	public:
		static Id loadFromString(QString const &string);
		static Id getRootId();

		explicit Id(QString const &editor = "", QString  const &diagram = "",
			QString  const &element = "", QString  const &id = "");
		Id(Id const &base, QString const &additional);
		QString editor() const;
		QString diagram() const;
		QString element() const;
		QString id() const;
		unsigned idSize() const;
		QUrl toUrl() const;
		QString toString() const;

		// applicable only to element's IDs. returnes type's ID
		Id type() const;

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

	inline bool operator==(const Id &i1, const Id &i2)
	{
		return i1.editor() == i2.editor()
			&& i1.diagram() == i2.diagram()
			&& i1.element() == i2.element()
			&& i1.id() == i2.id();
	}

	inline bool operator!=(const Id &i1, const Id &i2)
	{
		return !(i1 == i2);
	}

	inline uint qHash(const Id &key)
	{
		return qHash(key.editor()) ^ qHash(key.diagram()) ^ qHash(key.element())
				^ qHash(key.id());
	}

	typedef QList<Id> IdList;

	class IdListHelper {
	public:
		static QVariant toVariant(IdList const &list);
	};
}

// qReal::Id and qReal::IdList could be used straight in QVariant
Q_DECLARE_METATYPE(qReal::Id)

Q_DECLARE_METATYPE(qReal::IdList)
