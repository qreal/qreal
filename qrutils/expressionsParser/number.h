#pragma once

#include <QVariant>

#include "../utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT Number {
public:
	enum Type {
		doubleType = 0,
		intType
	};

public:
	Number(QVariant n, Type t);
	Number();
	~Number();

	QVariant property(QString name);
	void setProperty(QString name, QVariant value);
	QString toString() const;

	void operator+=(Number add);
	void operator-=(Number sub);
	void operator*=(Number mult);
	void operator/=(Number div);
	Number operator-();
	bool operator<(Number arg);
	bool operator>(Number arg);
	bool operator==(Number arg);
	bool operator<=(Number arg);
	bool operator>=(Number arg);
	bool operator!=(Number arg);

private:
	QVariant mNumber;
	Type mType;
};
}
