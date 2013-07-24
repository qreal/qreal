#include "abstractType.h"

using namespace utils;

AbstractType::~AbstractType()
{
}

AbstractType::Type AbstractType::type() const
{
	return mType;
}

void AbstractType::setType(Type t)
{
	mType = t;
}

QString AbstractType::typeToString()
{
	switch (mType) {
	case number:
		return "Number";
	default:
		return "Array";
	}
}
