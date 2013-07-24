#pragma once

#include "abstractType.h"
#include "number.h"
#include <QVariant>
#include <QVector>


#include "../utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT Array : public AbstractType{
public:
	enum Type {
		doubleType = 0,
		intType
	};

public:
	Array(QVector<Number> value, Type t);
	Array();
	~Array();

	QVariant evaluate();
	QVariant getAt(int number);
	QString toString() const;
	void writeAt(int count, int value);
	void setProperty(QString name, QVariant value);
	QVariant property(QString name);
	bool checkForOutOfRange(int number);
	int size();

	QVector <Number> mValue;
private:
	Type mType;
};
}

