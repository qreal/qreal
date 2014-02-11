#include "computableNumber.h"

using namespace utils;

ComputableNumber::ComputableNumber()
{
}

ComputableNumber::ComputableNumber(IntComputer const &intComputer)
	: Number(0, intType)
	, mComputer(intComputer)
{
}

QVariant ComputableNumber::property(QString const &name) const
{
	if (name != "Number") {
		return Number::property(name);
	}

	return mComputer();
}

QString ComputableNumber::toString() const
{
	return QString::number(mComputer());
}
