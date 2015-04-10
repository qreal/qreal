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

#include "intPropertyConverter.h"

#include <qrtext/lua/luaToolbox.h>
#include <qrtext/lua/types/integer.h>

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

IntPropertyConverter::IntPropertyConverter(const QString &pathToTemplates
		, lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QString &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter
		, simple::Binding::ConverterInterface *typeConverter)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
	, TemplateParametrizedEntity(pathToTemplates)
	, mTypeConverter(typeConverter)
{
}

IntPropertyConverter::~IntPropertyConverter()
{
	delete mTypeConverter;
}

QString IntPropertyConverter::convert(const QString &data) const
{
	const QString preparedCode = CodeConverterBase::convert(data);
	if (preparedCode.isEmpty()) {
		return "0";
	}

	if (mLuaTranslator.toolbox().type(mLuaTranslator.toolbox()
			.parse(mId, QString(), data))->is<qrtext::lua::types::Integer>())
	{
		return preparedCode;
	}

	QString castTemplate = readTemplate("types/cast.t");
	const QString intType = mTypeConverter->convert("int");
	castTemplate.replace("@@TYPE@@", intType);
	castTemplate.replace("@@EXPRESSION@@", preparedCode);
	return castTemplate;
}

