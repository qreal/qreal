#include "type.h"
#include "value.h"

using namespace qRealType;

QRealValue::QRealValue(QRealType *type)
{
	mType = type;
	// other fields will be initialized by qRealType::newValue()
}

QRealValue* QRealValue::clone() const
{
	QRealValue* result = new QRealValue(mType);
	result->mValue = mValue;
	return result;
}

QString QRealValue::toString() const
{
	return mType->toStringValue(this);
}

void QRealValue::fromString(QString const &val)
{
	mType->fromStringValue(this, val);
}

int QRealValue::toInteger() const
{
	return mType->toIntegerValue(this);
}

void QRealValue::fromInteger(int val)
{
	mType->fromIntegerValue(this, val);
}

bool QRealValue::toBoolean() const
{
	return mType->toBooleanValue(this);
}

void QRealValue::fromBoolean(bool val)
{
	mType->fromBooleanValue(this, val);
}

double QRealValue::toReal() const
{
	return mType->toRealValue(this);
}

void QRealValue::fromReal(double val)
{
	mType->fromRealValue(this, val);
}
