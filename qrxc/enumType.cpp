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

bool EnumType::generateEnumValues(utils::OutFile &out, bool isNotFirst)
{
	if (mValues.isEmpty()) {
		return false;
	}

	generateOneCase(out, isNotFirst);

	out() << "\t\treturn { ";
	QStringList pairs;
	for (const QString &name : mValues.keys()) {
		pairs << QString("qMakePair(QString(\"%1\"), tr(\"%2\"))").arg(name, mValues[name]);
	}

	out() << pairs.join(", ");
	out() << " };\n";
	return true;
}

void EnumType::generateOneCase(utils::OutFile &out, bool isNotFirst) const
{
	//QString name = NameNormalizer::normalize(qualifiedName());

	if (!isNotFirst) {
		out() << "\tif (name == \"" << NameNormalizer::normalize(name()) << "\")\n";
	} else {
		out() << "\telse if (name == \"" << NameNormalizer::normalize(name()) << "\")\n";
	}
}

void EnumType::generatePropertyTypes(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void EnumType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void EnumType::generateMouseGesturesMap(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void EnumType::generateExplosionsMap(utils::OutFile &out)
{
	Q_UNUSED(out)
}

bool EnumType::isEditable() const
{
	return mIsEditable;
}
