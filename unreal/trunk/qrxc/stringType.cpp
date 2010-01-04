#include "stringType.h"

#include <QDebug>

bool StringType::init(QDomElement const &element)
{
	if (NonGraphicType::init(element)) {
		mRegularExpression = element.firstChildElement("regularExpression").text();
		return true;
	}
	else
		return false;
}

Type* StringType::clone() const
{
	StringType *result = new StringType();
	Type::copyFields(result);
	result->mRegularExpression = mRegularExpression;
	return result;
}
