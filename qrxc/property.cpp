/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "property.h"

#include <QtCore/QDebug>

bool Property::init(const QDomElement &element)
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

bool Property::initReferenceType(QString typeName, const QDomElement &element)
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

bool Property::operator == (const Property &other) const
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

bool Property::operator != (const Property &other) const
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

