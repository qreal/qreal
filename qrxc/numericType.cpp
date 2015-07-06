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

#include "numericType.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

bool NumericType::init(const QDomElement &element, const QString &context)
{
	if (NonGraphicType::init(element, context)) {
		QString baseTypeName = element.firstChildElement("base_type").text();
		if (baseTypeName == "int") {
			mBaseType = intType;
		} else if (baseTypeName == "float") {
			mBaseType = floatType;
		} else {
			qDebug() << "ERROR: unknown numeric base type" << baseTypeName;
			return false;
		}
		return true;
	}

	return false;
}

Type* NumericType::clone() const
{
	NumericType *result = new NumericType();
	Type::copyFields(result);
	result->mBaseType = mBaseType;
	return result;
}

bool NumericType::generateEnumValues(utils::OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out);
	Q_UNUSED(isNotFirst);
	return false;
}

void NumericType::generatePropertyTypes(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void NumericType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void NumericType::generateMouseGesturesMap(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void NumericType::generateExplosionsMap(utils::OutFile &out)
{
	Q_UNUSED(out)
}

