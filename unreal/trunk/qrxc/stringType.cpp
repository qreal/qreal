#include "stringType.h"

#include <QDebug>

bool StringType::init(QDomElement const &element)
{
	if (NonGraphicType::init(element))
	{
		mRegularExpression = element.firstChildElement("regularExpression").text();
		return true;
	} 
	else
	{
		return false;
	}
}