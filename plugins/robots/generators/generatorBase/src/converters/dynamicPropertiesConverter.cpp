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

#include "dynamicPropertiesConverter.h"

#include <QtXml/QDomDocument>

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

DynamicPropertiesConverter::DynamicPropertiesConverter(lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QStringList &pathsToTemplates
		, ConverterInterface *reservedVariablesConverter)
	: TemplateParametrizedEntity(pathsToTemplates)
	, mLuaTranslator(luaTranslator)
	, mId(id)
	, mReservedVariablesConverter(reservedVariablesConverter)
{
}

DynamicPropertiesConverter::~DynamicPropertiesConverter()
{
	delete mReservedVariablesConverter;
}

QString DynamicPropertiesConverter::convert(const QString &properties) const
{
	if (!properties.isEmpty()) {
		QDomDocument dynamicProperties;
		dynamicProperties.setContent(properties);

		QStringList argumentsList;
		for (QDomElement element
				= dynamicProperties.firstChildElement("properties").firstChildElement("property")
				; !element.isNull()
				; element = element.nextSiblingElement("property"))
		{
			argumentsList << mLuaTranslator.translate(element.attribute("dynamicPropertyValue")
					, mId, element.attribute("name"), mReservedVariablesConverter);
		}

		QString result = argumentsList.join(readTemplate("luaPrinting/argumentsSeparator.t"));
		return result;
	}

	return QString();
}
