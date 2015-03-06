#include "property.h"
#include "../utils/defs.h"

#include <QDebug>

using namespace qrmc;

Property::Property(qrRepo::LogicalRepoApi *api, const qReal::Id &id) : mApi(api), mId(id)
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
	mType = mApi->stringProperty(mId, "attributeType");
	if (mType.isEmpty()) {
		qDebug() << "ERROR: empty type of property found";
		return false;
	} else {
		qReal::const IdList listOfEnums = mApi->elementsByType("MetaEntityEnum");
		foreach (const qReal::Id enumElement, listOfEnums) {
			const QString nameOfEnumElement = mApi->name(enumElement);
			if (nameOfEnumElement == mType) {
				mIsEnum = true;
			}
		}
		qReal::const IdList listOfNodes = mApi->elementsByType("MetaEntityNode");
		foreach (const qReal::Id nodeElement, listOfNodes) {
			const QString nameOfNodeElement = mApi->name(nodeElement);
			if (nameOfNodeElement == mType) {
				mIsReference = true;
			}
		}
	}

	mDisplayedName = mApi->stringProperty(mId, "displayedName");
	mDefaultValue = mApi->stringProperty(mId, "defaultValue");
	return true;
}


QString Property::name() const
{
	return mName;
}

QString Property::type() const
{
	return mType;
}

QString Property::defaultValue() const
{
	return mDefaultValue;
}

Property * Property::clone()
{
	Property *result = new Property(mApi, mId);
	result->mName = mName;
	result->mType = mType;
	result->mIsEnum = mIsEnum;
	result->mIsReference = mIsReference;
	result->mDescription = mDescription;
	result->mDefaultValue = mDefaultValue;
	result->mDisplayedName = mDisplayedName;
	return result;
}

bool Property::operator == (Property const &other) const
{
	return other.mName == mName
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

bool Property::isReferenceProperty() const
{
	return mIsReference;
}

void Property::print() const
{
	qDebug() << "property"
			<< "\t" << mName
			<< "\t" << mType
			<< "\t" << mIsEnum
			<< "\t" << mIsReference
			<< "\t" << mDescription
			<< "\t" << mDefaultValue
			<< "\t" << mDisplayedName;
}

QString Property::generatePropertyLine(const QString &lineTemplate) const
{
	QString result = lineTemplate;
	result.replace(propertyNameTag, mName).replace(propertyTypeTag, mType);
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

QString Property::generateDisplayedNameLine(const QString &lineTemplate) const
{
	QString result = lineTemplate;
	result.replace(propertyNameTag, mName).replace(propertyDisplayedNameTag, mDisplayedName);
	return result;
}
