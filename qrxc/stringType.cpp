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

#include "stringType.h"

#include <QtCore/QDebug>

#include <qrutils/outFile.h>

bool StringType::init(const QDomElement &element, const QString &context)
{
	if (NonGraphicType::init(element, context)) {
		mRegularExpression = element.firstChildElement("regularExpression").text();
		return true;
	}
	else
		return false;
}

Type* StringType::clone() const
{
	StringType *result = new StringType();
	Type::copyFields(result);
	result->mRegularExpression = mRegularExpression;
	return result;
}

bool StringType::generateEnumValues(utils::OutFile &out, bool isNotFirst)
{
	Q_UNUSED(out);
	Q_UNUSED(isNotFirst);
	return false;
}

void StringType::generatePropertyTypes(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void StringType::generatePropertyDefaults(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void StringType::generateMouseGesturesMap(utils::OutFile &out)
{
	Q_UNUSED(out);
}

void StringType::generateExplosionsMap(utils::OutFile &out)
{
	Q_UNUSED(out)
}
