#pragma once

#include <QDomElement>
#include <QStringList>

class NumericType {
public:
	NumericType(){}
	~NumericType(){}
	bool init(QDomElement&);
};

class StringType {
public:
	StringType(){}
	~StringType(){}
	bool init(QDomElement&);
};

class EnumType {
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
