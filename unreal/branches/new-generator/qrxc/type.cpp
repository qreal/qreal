#include "type.h"
#include "property.h"

#include <qDebug>

Type::Type(bool isResolved)
	: mResolvingFinished(isResolved)
{}

Type::~Type()
{
	foreach (Property *property, mProperties.values())
	{
		if (property)
		{
			delete property;
		}
	}
}

bool Type::init(QDomElement const &element)
{
	mName = element.attribute("name");
	if (mName == "")
	{
		qDebug() << "Error: anonymous type found";
		return false;
	}
	return true;
}

bool Type::isResolved()
{
	return mResolvingFinished;
}

QString Type::name()
{
	return mName;
}

QMap<QString, Property*> Type::properties()
{
	return mProperties;
}
