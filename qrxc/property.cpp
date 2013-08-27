#include "property.h"

#include <QtCore/QDebug>

bool Property::init(QDomElement const &element)
{
	mIsEnum = false;
	mIsReference = false;
	mName = element.attribute("name");
	if (mName.isEmpty()) {
		qDebug() << "ERROR: anonymous property found";
		return false;
	}
	mType = element.attribute("type");
	if (mType.isEmpty()) {
		qDebug() << "ERROR: no type of property found";
		return false;
	} else if (mType == "enum") {
		if (initReferenceType("enum", element))
			mIsEnum = true;	// TODO: Lookup enum
		else
			return false;
	} else if (mType == "reference") {
		if (initReferenceType("reference", element)) {
			mIsReference = true;  // TODO: Lookup reference
		} else {
			return false;
		}
	}

	mDisplayedName = element.attribute("displayedName");
	mDescription = element.firstChildElement("description").text();
	mDefaultValue = element.firstChildElement("default").text();
	return true;
}

bool Property::initReferenceType(QString typeName, QDomElement const &element)
{
	mType = element.firstChildElement(typeName).attribute("type");
	if (mType.isEmpty()) {
		qDebug() << "ERROR: anonymous property reference type for" << name() << "found";
		return false;
	}
	return true;
}

QString Property::name() const
{
	return mName;
}

QString Property::displayedName() const
{
	return mDisplayedName;
}

QString Property::type() const
{
	return mType;
}

bool Property::isReferenceProperty() const
{
	return mIsReference;
}

QString Property::defaultValue() const
{
	return mDefaultValue;
}

QString Property::description() const
{
	return mDescription;
}

Property * Property::clone()
{
	Property *result = new Property();
	result->mName = mName;
	result->mDisplayedName = mDisplayedName;
	result->mType = mType;
	result->mIsEnum = mIsEnum;
	result->mIsReference = mIsReference;
	result->mDescription = mDescription;
	result->mDefaultValue = mDefaultValue;
	return result;
}

bool Property::operator == (Property const &other) const
{
	return other.mName == mName
		&& other.mDisplayedName == mDisplayedName
		&& other.mType == mType
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
	qDebug() << mDisplayedName;
	qDebug() << mType;
	qDebug() << mIsEnum;
	qDebug() << mIsReference;
	qDebug() << mDescription;
	qDebug() << mDefaultValue;
}

