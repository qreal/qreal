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

QVariant ComputableNumber::value() const
{
	return mComputer();
}
