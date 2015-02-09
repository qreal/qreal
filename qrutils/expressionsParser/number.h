#pragma once

#include <QtCore/QVariant>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT Number
{
public:
	enum Type {
		doubleType = 0
		, intType
	};

public:
	Number(const QVariant &value, Type type);
	Number();
	virtual ~Number();

	Type type() const;
	void setType(Type type);

	virtual QVariant value() const;
	void setValue(const QVariant &value);

	QString toString() const;

	void operator+=(const Number &add);
	void operator-=(const Number &sub);
	void operator*=(const Number &mult);
	void operator/=(const Number &div);
	Number operator-();
	bool operator<(const Number &arg);
	bool operator>(const Number &arg);
	bool operator==(const Number &arg);
	bool operator<=(const Number &arg);
	bool operator>=(const Number &arg);
	bool operator!=(const Number &arg);

protected:
	QVariant mValue;
	Type mType;
};

}
