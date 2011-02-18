#include "property.h"
#include "utils/defs.h"

#include <QDebug>

using namespace qrmc;

Property::Property(qrRepo::RepoApi *api, qReal::Id const &id) : mApi(api), mId(id)
{

}

bool Property::init()
{
	mIsEnum = false;
	mIsReference = false;
	mName = mApi->name(mId);
	if (mName.isEmpty()) {
		qDebug() << "ERROR: anonymous property found";
		return false;
	}
        mTypeString = mApi->stringProperty(mId, "attributeType");
        if (mTypeString.isEmpty()) {
		qDebug() << "ERROR: empty type of property found";
		return false;
	}

	mDefaultValue = mApi->stringProperty(mId, "defaultValue");
	return true;
}


QString Property::name()
{
	return mName;
}

QString Property::type()
{
        return mTypeString;
}

QString Property::defaultValue()
{
	return mDefaultValue;
}

Property * Property::clone()
{
	Property *result = new Property(mApi, mId);
	result->mName = mName;
        result->mId = mId;
	result->mIsEnum = mIsEnum;
	result->mIsReference = mIsReference;
	result->mDescription = mDescription;
	result->mDefaultValue = mDefaultValue;
	return result;
}

bool Property::operator == (Property const &other) const
{
	return other.mName == mName
                && other.mId == mId
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

void Property::print() const
{
	qDebug() << "property"
			<< "\t" << mName
                        << "\t" << mTypeString
			<< "\t" << mIsEnum
			<< "\t" << mIsReference
			<< "\t" << mDescription
			<< "\t" << mDefaultValue;
}

QString Property::generatePropertyLine(const QString &lineTemplate) const
{
	QString result = lineTemplate;
        result.replace(propertyNameTag, mName).replace(propertyTypeTag, mTypeString);
	return result;
}

QString Property::generateDefaultsLine(const QString &lineTemplate) const
{
	if (mDefaultValue.isEmpty())
		return "";
	QString result = lineTemplate;
	result.replace(propertyNameTag, mName).replace(propertyDefaultTag, mDefaultValue);
	return result;
}
