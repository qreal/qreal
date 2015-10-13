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
#include "../utils/nameNormalizer.h"

using namespace qReal;
using namespace qrmc;

EnumType::EnumType(Diagram *diagram, qrRepo::LogicalRepoApi *api, const qReal::Id &id) : NonGraphicType(diagram, api, id)
{
}

bool EnumType::init(const QString &context)
{
	Type::init(context);
	IdList children = mApi->children(mId);
	foreach(Id child, children) {
		if (!mApi->isLogicalElement(child))
			continue;
		if (child.element() == metaEntityValue) {
			const QString name = mApi->stringProperty(child, "valueName");
			const QString displayedName = mApi->stringProperty(child, "displayedName");
			mValues[name] = displayedName;
		}
	}

	return true;
}

Type* EnumType::clone() const
{
	EnumType *result = new EnumType(nullptr, mApi, mId);
	Type::copyFields(result);
	result->mValues = mValues;
	return result;
}

void EnumType::print()
{
	qDebug() << "enum type" << mName;
}

QString EnumType::generateEnums(const QString &lineTemplate) const
{
	QString line = lineTemplate;
	QString lineForWrite = "";
	for (const QString &value : mValues.keys()) {
		if (!lineForWrite.isEmpty()) {
			lineForWrite += ", qMakePair(QString(\"" + value + "\"), tr(\"" + mValues[value] + "\"))";
		} else {
			lineForWrite = "qMakePair(QString(\"" + value + "\"), tr(\"" + mValues[value] + "\"))";
		}
	}
	line.replace(qMakeLineTag, lineForWrite).replace(elementNameTag, name());

	return line;
}
