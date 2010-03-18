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
	// TODO
	return QString();
}

void QRealValue::fromString(QString const &val)
{
	// TODO
	Q_UNUSED(val);
}

int QRealValue::toInteger() const
{
	// TODO
	return 0;
}

void QRealValue::fromInteger(int val)
{
	// TODO
	Q_UNUSED(val);
}

bool QRealValue::toBoolean() const
{
	// TODO
	return false;
}

void QRealValue::fromBoolean(bool val)
{
	// TODO
	Q_UNUSED(val);
}

double QRealValue::toReal() const
{
	// TODO
	return 0.0;
}

void QRealValue::fromReal(double val)
{
	// TODO
	Q_UNUSED(val);
}
