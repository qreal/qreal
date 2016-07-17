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

#include "generatorBase/converters/enumConverterBase.h"

using namespace generatorBase::converters;
using namespace qReal;

EnumConverterBase::EnumConverterBase(const QStringList &pathsToTemplates
		, QMap<QString, QString> const &valuesToTemplatePaths)
	: TemplateParametrizedConverter(pathsToTemplates)
	, mValuesToTemplates(valuesToTemplatePaths)
{
}

void EnumConverterBase::addMapping(const QString &enumValue, const QString &templatePath)
{
	mValuesToTemplates[enumValue] = templatePath;
}

QString EnumConverterBase::convert(const QString &data) const
{
	foreach (const QString &value, mValuesToTemplates.keys()) {
		if (value == data) {
			return readTemplate(mValuesToTemplates[value]);
		}
	}

	return QString();
}
