#include "property.h"

#include <qDebug>

bool Property::init(QDomElement const &element)
{
	mIsEnum = false;
	mIsReference = false;
	mName = element.attribute("name");
	if (mName == "")
	{
		qDebug() << "Error: anonymous property found";
		return false;
	}
	QString mType = element.attribute("type");
	if (mType == "")
	{
		qDebug() << "Error: no type of property found";
		return false;
	}
	else if (mType == "enum")
	{
		if (initReferenceType("enum", element))
			mIsEnum = true;	// TODO: Lookup enum
		else 
			return false;
	} 
	else if (mType == "reference")
	{				
		if (initReferenceType("reference", element))
			mIsReference = true; // TODO: Lookup reference
		else
			return false;
	} 
	else if ((mType != "int") && (mType != "string") && (mType != "bool") && (mType != "text") &&
		(mType != "positiveInt") && (mType != "nonNegativeInt"))
	{
		qDebug() << "Error: unknown type found";
		return false;
	}
	mDescription = element.firstChildElement("description").text();
	mDefaultValue = element.firstChildElement("default").text();
	return true;
}

bool Property::initReferenceType(QString typeName, QDomElement const &element)
{
	mType = element.firstChildElement(typeName).attribute("nameReference");
	if (mType == "")
	{
		qDebug() << "Error: anonymous type found";
		return false;
	}
	return true;
}

QString Property::name()
{
	return mName;
}