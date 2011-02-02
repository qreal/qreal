#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QMetaType>

namespace qReal {

	class Id {
	public:
                explicit Id(QString  const &id = "");
		Id(Id const &base, QString const &additional);

		QString id() const;
		QString toString() const;

		// default destructor and copy constuctor are OK
	private:
		QString mId;

		// used only for debug
		bool checkIntegrity() const;
	};

	inline bool operator==(const Id &i1, const Id &i2)
	{
                return 	i1.id() == i2.id();
	}

	inline bool operator!=(const Id &i1, const Id &i2)
	{
		return !(i1 == i2);
	}

	inline uint qHash(const Id &key)
	{
                return qHash(key.id());
	}

}

// qReal::Id and qReal::IdList could be used straight in QVariant
Q_DECLARE_METATYPE(qReal::Id)

