#include "property.h"
#include "../utils/defs.h"

#include <QDebug>

using namespace qrmc;

Property::Property(qrRepo::LogicalRepoApi *api, qReal::Id const &id) : mApi(api), mId(id)
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
		qReal::IdList const listOfEnums = mApi->elementsByType("MetaEntityEnum");
		foreach (qReal::Id const enumElement, listOfEnums) {
			QString const nameOfEnumElement = mApi->name(enumElement);
			if (nameOfEnumElement == mType) {
				mIsEnum = true;
			}
		}
		qReal::IdList const listOfNodes = mApi->elementsByType("MetaEntityNode");
		foreach (qReal::Id const nodeElement, listOfNodes) {
			QString const nameOfNodeElement = mApi->name(nodeElement);
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

QString Property::generatePropertyLine(QString const &lineTemplate) const
{
	QString result = lineTemplate;
	result.replace(propertyNameTag, mName).replace(propertyTypeTag, mType);
	return result;
}

QString Property::generateDefaultsLine(QString const &lineTemplate) const
{
	if (mDefaultValue.isEmpty())
		return "";
	QString result = lineTemplate;
	result.replace(propertyNameTag, mName).replace(propertyDefaultTag, mDefaultValue);
	return result;
}

QString Property::generateDisplayedNameLine(QString const &lineTemplate) const
{
	QString result = lineTemplate;
	result.replace(propertyNameTag, mName).replace(propertyDisplayedNameTag, mDisplayedName);
	return result;
}
