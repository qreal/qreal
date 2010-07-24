#include "enumType.h"
#include "utils/nameNormalizer.h"
#include "utils/outFile.h"

bool EnumType::init()
{
	return true;
}

Type* EnumType::clone() const
{
	EnumType *result = new EnumType();
	Type::copyFields(result);
	result->mValues = mValues;
	return result;
}

void EnumType::print()
{
	qDebug() << "enum type" << mName;
}
