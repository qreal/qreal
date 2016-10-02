/* Copyright 2016 Julia Khramyshkina, QReal Research Group
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

#include "roleType.h"

bool RoleType::init(const QDomElement &element, const QString &context)
{
	if (!NonGraphicType::init(element, context)) {
		return false;
	}

	mArrowType = element.attribute("arrowType");
	mEnd = element.attribute("end");
	mNavigable = element.attribute("navigable");

	QDomElement first = element.firstChildElement("properties");

	for (QDomElement propertyElement = first.firstChildElement("property")
		 ; !propertyElement.isNull()
		 ; propertyElement = propertyElement.nextSiblingElement("property"))
	{
		Property *property = new Property();
		property->init(propertyElement);
		mProperties.append(property);
	}

	return true;
}

RoleType* RoleType::clone() const
{
	RoleType *result = new RoleType();
	result->mArrowType = mArrowType;
	result->mEnd = mEnd;
	result->mNavigable = mNavigable;
	result->mProperties = mProperties;
	Type::copyFields(result);
	return result;
}

bool RoleType::isEnding()
{
	return mEnd == "true";
}

bool RoleType::isNavigable()
{
	return mNavigable == "true";
}

QString RoleType::typeOfArrow()
{
	return mArrowType;
}

QList<Property*> RoleType::getPropertiesOfRole()
{
	return mProperties;
}
