#include "number.h"

using namespace utils;

Number::Number(QVariant const &n, Type t)
	: mNumber(n)
	, mType(t)
{
}

Number::Number()
{
	mType = Number::intType;
	mNumber = 0;
}

Number::~Number()
{
}

QVariant Number::property(QString const &name) const
{
	if (name.compare("Number") == 0) {
		return mNumber;
	} else if (name.compare("Type") == 0) {
		return mType;
	}

	return QVariant();
}

void Number::setProperty(QString const &name, QVariant const &value)
{
	if (name.compare("Number") == 0) {
		mNumber = value;
	} else if (name.compare("Type") == 0) {
		mType = value.toInt() ? Number::intType : Number::doubleType;
	}
}

QString Number::toString() const
{
	return mNumber.toString();
}

void Number::operator+=(Number const &add)
{
	Number::Type t = add.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = add.property("Number");
	if (mType == t) {
		switch (t) {
			case Number::intType:
				mNumber = mNumber.toInt() + val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() + val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() + val.toDouble();
	}
}

void Number::operator-=(Number const &sub)
{
	Number::Type t = sub.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = sub.property("Number");
	if (mType == t) {
		switch (t) {
			case Number::intType:
				mNumber = mNumber.toInt() - val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() - val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() - val.toDouble();
	}
}

void Number::operator*=(Number const &mult)
{
	Number::Type t = mult.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = mult.property("Number");
	if (mType == t) {
		switch (t) {
			case Number::intType:
				mNumber = mNumber.toInt() * val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() * val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() * val.toDouble();
	}
}

void Number::operator/=(Number const &div)
{
	Number::Type t = div.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = div.property("Number");
	if (mType == t) {
		switch (t) {
			case Number::intType:
				mNumber = mNumber.toInt() / val.toInt();
				break;
			case Number::doubleType:
				mNumber = mNumber.toDouble() / val.toDouble();
				break;
		}
	} else {
		mType = Number::doubleType;
		mNumber = mNumber.toDouble() / val.toDouble();
	}
}

Number Number::operator-()
{
	switch (mType) {
		case Number::intType:
			mNumber = -mNumber.toInt();
			break;
		case Number::doubleType:
			mNumber = -mNumber.toDouble();
			break;
	}

	return *this;
}

bool Number::operator<(Number const &arg)
{
	Number::Type t = arg.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = arg.property("Number");
	if (mType == Number::intType && t == Number::intType) {
		return mNumber.toInt() < val.toInt();
	} else if (mType == Number::intType && t == Number::doubleType) {
		return mNumber.toInt() < val.toDouble();
	} else if (mType == Number::doubleType && t == Number::intType) {
		return mNumber.toDouble() < val.toInt();
	} else {
		return mNumber.toDouble() < val.toDouble();
	}
}

bool Number::operator==(Number const &arg)
{
	Number::Type t = arg.property("Type").toInt() ? Number::intType : Number::doubleType;
	QVariant val = arg.property("Number");
	if (mType == Number::intType && t == Number::intType) {
		return mNumber.toInt() == val.toInt();
	} else if (mType == Number::intType && t == Number::doubleType) {
		return mNumber.toInt() == val.toDouble();
	} else if (mType == Number::doubleType && t == Number::intType) {
		return mNumber.toDouble() == val.toInt();
	} else {
		return mNumber.toDouble() == val.toDouble();
	}
}

bool Number::operator>(Number const &arg)
{
	return !((*this)<arg || (*this)==arg);
}

bool Number::operator<=(Number const &arg)
{
	return (*this)<arg || (*this)==arg;
}

bool Number::operator>=(Number const &arg)
{
	return !((*this)<arg);
}

bool Number::operator!=(Number const &arg)
{
	return !((*this)==arg);
}
