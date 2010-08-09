#include "property.h"

#include <QDebug>

bool Property::init(QDomElement const &element)
{
	mIsEnum = false;
	mIsReference = false;
	mName = element.attribute("name");
	if (mName.isEmpty())
	{
		qDebug() << "ERROR: anonymous property found";
		return false;
	}
	QString mType = element.attribute("type");
// Ad kalendas graecas
#if 0
	mRealType = qRealType::QRealTypeFactory::getTypeByName(mType);
	if (!mRealType)
	{
		qDebug() << "WARNING: type " << mType << " is not registered";
		// For now just print a warning
	}
#endif
	if (mType.isEmpty())
	{
		qDebug() << "ERROR: no type of property found";
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
		// Хак: проперти непримитивных типов пока что будут строковыми, это лучше,
		// чем если их не будет вообще.
		mType = "string";
		// qDebug() << "ERROR: unknown type" << mType << "found for property" << name();
		// return false;
	}
	mDescription = element.firstChildElement("description").text();
	mDefaultValue = element.firstChildElement("default").text();
	return true;
}

bool Property::initReferenceType(QString typeName, QDomElement const &element)
{
	mType = element.firstChildElement(typeName).attribute("nameReference");
	if (mType.isEmpty())
	{
		qDebug() << "ERROR: anonymous property reference type for" << name() << "found";
		return false;
	}
	return true;
}

QString Property::name()
{
	return mName;
}

Property * Property::clone()
{
	Property *result = new Property();
	result->mName = mName;
	result->mType = mType;
	// Ad kalendas graecas
	//result->mRealType = mRealType;
	result->mIsEnum = mIsEnum;
	result->mIsReference = mIsReference;
	result->mDescription = mDescription;
	result->mDefaultValue = mDefaultValue;
	return result;
}

bool Property::operator == (Property const &other) const
{
	return other.mName == mName
		&& other.mType == mType
		// Ad kalendas graecas
		//&& other.mRealType == mRealType
		&& other.mIsEnum == mIsEnum
		&& other.mIsReference == mIsReference
		&& other.mDescription == mDescription
		&& other.mDefaultValue == mDefaultValue
		;
}

bool Property::operator != (Property const &other) const
{
	return !(other == *this);
}

void Property::dump() const
{
	qDebug() << mName;
	qDebug() << mType;
	// Ad kalendas graecas
	//qDebug() << mRealType;
	qDebug() << mIsEnum;
	qDebug() << mIsReference;
	qDebug() << mDescription;
	qDebug() << mDefaultValue;
}

