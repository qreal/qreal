#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

namespace qReal {

class Id {
public:
	static Id loadFromString(QString const &string);
	explicit Id(QString const &id = "");
	Id(Id const &base, QString const &additional);

	QString id() const;
	QString toString() const;
	QVariant toVariant() const;

	// default destructor and copy constuctor are OK
private:
	QString mId;

	// used only for debug
	bool checkIntegrity() const;
};

inline bool operator==(Id const &i1, Id const &i2)
{
	return 	i1.id() == i2.id();
}

inline bool operator!=(Id const &i1, Id const &i2)
{
	return !(i1 == i2);
}

inline uint qHash(Id const &key)
{
	return qHash(key.id());
}

inline QDebug operator<<(QDebug dbg, Id const &id)
{
	dbg << id.toString();
	return dbg.space();
}

typedef QList<Id> IdList;

class IdListHelper {
public:
	static QVariant toVariant(IdList const &list);
};


typedef Id Metatype;
}

// qReal::Id and qReal::IdList could be used straight in QVariant
Q_DECLARE_METATYPE(qReal::Id)

Q_DECLARE_METATYPE(qReal::IdList)

