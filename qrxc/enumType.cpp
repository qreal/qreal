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

#include "enumType.h"

#include <QtCore/QStringList>

#include <qrutils/outFile.h>

#include "nameNormalizer.h"

bool EnumType::init(const QDomElement &element, const QString &context)
{
	if (!NonGraphicType::init(element, context)) {
		return false;
	}

	mIsEditable = element.attribute("editable").toLower() == "true";

	for (QDomElement valueElement = element.firstChildElement("value")
		; !valueElement.isNull()
		; valueElement = valueElement.nextSiblingElement("value"))
	{
		const QString name = valueElement.attribute("name");
		QString displayedName = valueElement.attribute("displayedName");
		if (displayedName.isEmpty()) {
			displayedName = name;
		}

		mValues[name] = displayedName;
	}

	return true;
}

Type* EnumType::clone() const
{
	EnumType *result = new EnumType();
	Type::copyFields(result);
	result->mValues = mValues;
	return result;
}

const QMap<QString, QString> &EnumType::values() const
{
	return mValues;
}

bool EnumType::isEditable() const
{
	return mIsEditable;
}
