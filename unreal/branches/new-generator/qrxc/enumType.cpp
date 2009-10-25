#include "enumType.h"

#include <qDebug>

bool EnumType::init(QDomElement const &element)
{
	if (NonGraphicType::init(element))
	{
		for (QDomElement valueElement = element.firstChildElement("value"); !valueElement.isNull();
			valueElement = valueElement.nextSiblingElement("value"))
		{
			mValues.append(valueElement.text());
		}
		return true;
	} 
	else
	{
		return false;
	}
}