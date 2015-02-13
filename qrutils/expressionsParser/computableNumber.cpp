#include "computableNumber.h"

using namespace utils;

ComputableNumber::ComputableNumber()
{
}

ComputableNumber::ComputableNumber(const IntComputer &intComputer)
	: Number(0, intType)
	, mComputer(intComputer)
{
}

QVariant ComputableNumber::value() const
{
	return mComputer();
}
