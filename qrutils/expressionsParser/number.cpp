#include "number.h"

#include <qrkernel/exception/exception.h>

#include "mathUtils/math.h"

using namespace utils;

Number::Number(const QVariant &value, Type type)
	: mValue(value)
	, mType(type)
{
}

Number::Number()
{
	mType = Number::intType;
	mValue = 0;
}

Number::~Number()
{
}

Number::Type Number::type() const
{
	return mType;
}

void Number::setType(Number::Type type)
{
	mType = type;
}

QVariant Number::value() const
{
	return mValue;
}

void Number::setValue(const QVariant &value)
{
	mValue = value;
}

QStringList Number::toStringList() const
{
	return value().toStringList();
}

void Number::operator+=(const Number &add)
{
	if (mType == intType && add.type() == intType) {
		mValue = mValue.toInt() + add.value().toInt();
	} else {
		mType = Number::doubleType;
		mValue = mValue.toDouble() + add.value().toDouble();
	}
}

void Number::operator-=(const Number &sub)
{
	if (mType == intType && sub.type() == intType) {
		mValue = mValue.toInt() - sub.value().toInt();
	} else {
		mType = Number::doubleType;
		mValue = mValue.toDouble() - sub.value().toDouble();
	}
}

void Number::operator*=(const Number &mult)
{
	if (mType == intType && mult.type() == intType) {
		mValue = mValue.toInt() * mult.value().toInt();
	} else {
		mType = Number::doubleType;
		mValue = mValue.toDouble() * mult.value().toDouble();
	}
}

void Number::operator/=(const Number &div)
{
	if (mType == intType && div.type() == intType) {
		if (div.value().toInt() == 0) {
			throw qReal::Exception("Division by zero");
		}

		mValue = mValue.toInt() / div.value().toInt();
	} else {
		mType = Number::doubleType;
		mValue = mValue.toDouble() / div.value().toDouble();
	}
}

Number Number::operator-()
{
	switch (mType) {
		case Number::intType:
			mValue = -mValue.toInt();
			break;
		case Number::doubleType:
			mValue = -mValue.toDouble();
			break;
	}

	return *this;
}

bool Number::operator<(const Number &arg)
{
	return mValue.toDouble() < arg.value().toDouble();
}

bool Number::operator==(const Number &arg)
{
	const Number::Type argType = arg.type();
	if (mType == Number::intType && argType == Number::intType) {
		return mValue.toInt() == arg.value().toInt();
	} else if (mType == Number::intType && argType == Number::doubleType) {
		return mValue.toInt() == arg.value().toDouble();
	} else if (mType == Number::doubleType && argType == Number::intType) {
		return mValue.toDouble() == arg.value().toInt();
	} else {
		return mathUtils::Math::eq(mValue.toDouble(), arg.value().toDouble());
	}
}

bool Number::operator>(const Number &arg)
{
	return !((*this)<arg || (*this)==arg);
}

bool Number::operator<=(const Number &arg)
{
	return (*this)<arg || (*this)==arg;
}

bool Number::operator>=(const Number &arg)
{
	return !((*this)<arg);
}

bool Number::operator!=(const Number &arg)
{
	return !((*this)==arg);
}
