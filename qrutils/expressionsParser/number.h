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
	Number(QVariant const &value, Type type);
	Number();
	virtual ~Number();

	Type type() const;
	void setType(Type type);

	virtual QVariant value() const;
	void setValue(QVariant const &value);

	QString toString() const;

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
	QVariant mValue;
	Type mType;
};

}
