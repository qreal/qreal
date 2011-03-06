#pragma once

#include <QVariant>

namespace qReal {
class Number {
public:
	enum Type {
		doubleType = 0,
		intType
	};

public:
	explicit Number(QVariant n, Type t);
	explicit Number();
	~Number();

	QVariant property(QString name);
	void setProperty(QString name, QVariant value);

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
