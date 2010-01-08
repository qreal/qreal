#include "enumType.h"

#include <QDebug>

bool EnumType::init(QDomElement const &element, QString const &context)
{
	if (NonGraphicType::init(element, context)) {
		for (QDomElement valueElement = element.firstChildElement("value"); !valueElement.isNull();
			valueElement = valueElement.nextSiblingElement("value"))
		{
			mValues.append(valueElement.text());
		}
		return true;
	} else
		return false;
}

Type* EnumType::clone() const
{
	EnumType *result = new EnumType();
	Type::copyFields(result);
	result->mValues = mValues;
	return result;
}
