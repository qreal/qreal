#include "type.h"
#include "value.h"

using namespace qRealType;

qRealValue::qRealValue(qRealType *type_)
{
	type = type_;
	// other fields will be initialized by qRealType::New()
}

qRealValue& qRealValue::clone()
{
	qRealValue result = type->New();
	result.value = value;
	return result;
}

QString qRealValue::toString()
{
	// TODO
}

void qRealValue::fromString(QString const &val)
{
	// TODO
}

int qRealValue::toInteger()
{
	// TODO
}

void qRealValue::fromInteger(int val)
{
	// TODO
}

bool qRealValue::toBoolean()
{
	// TODO
}

void qRealValue::fromBoolean(bool val)
{
	// TODO
}

double qRealValue::toReal()
{
	// TODO
}

void qRealValue::fromReal(double val)
{
	// TODO
}
