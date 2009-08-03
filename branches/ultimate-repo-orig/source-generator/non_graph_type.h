#pragma once

#include <QDomElement>
#include <QStringList>

enum {
	QREAL_NG_NUMERIC_INT,
	QREAL_NG_NUMERIC_BOOL,
	QREAL_NG_NUMERIC_FLOAT,
};

class NumericType {
	QString name;
	int base_type;
public:
	NumericType(){}
	~NumericType(){}
	bool init(QDomElement&);
};

class StringType {
	QString name;
	QString regexp;
public:
	StringType(){}
	~StringType(){}
	bool init(QDomElement&);
};

class EnumType {
	QString name;
	QStringList values;
public:
	EnumType(){}
	~EnumType(){}
	bool init(QDomElement&);
};

enum {
	QREAL_NG_INVALID = 0,
	QREAL_NG_NUMERIC,
	QREAL_NG_STRING,
	QREAL_NG_ENUM,
};

class NonGraphType {
public:
	int type;
	NumericType *n;
	StringType *s;
	EnumType *e;

	NonGraphType(){type = QREAL_NG_INVALID;}
	NonGraphType(NumericType *t) {type = QREAL_NG_NUMERIC; n = t;}
	NonGraphType(StringType *t) {type = QREAL_NG_STRING; s = t;}
	NonGraphType(EnumType *t) {type = QREAL_NG_ENUM; e = t;}
	~NonGraphType(){}
};
