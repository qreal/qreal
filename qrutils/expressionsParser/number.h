#pragma once

#include <QtCore/QVariant>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT Number
{
public:
	enum Type {
		doubleType = 0,
		intType
	};

public:
	Number(QVariant const &n, Type t);
	Number();
	virtual ~Number();

	virtual QVariant property(QString const &name) const;
	void setProperty(QString const &name, QVariant const &value);
	virtual QString toString() const;

	void operator+=(Number const &add);
	void operator-=(Number const &sub);
	void operator*=(Number const &mult);
	void operator/=(Number const &div);
	Number operator-();
	bool operator<(Number const &arg);
	bool operator>(Number const &arg);
	bool operator==(Number const &arg);
	bool operator<=(Number const &arg);
	bool operator>=(Number const &arg);
	bool operator!=(Number const &arg);

protected:
	QVariant mNumber;
	Type mType;
};

}
