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

#include "boolPropertyConverter.h"

#include <qrtext/lua/types/boolean.h>

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

BoolPropertyConverter::BoolPropertyConverter(const QStringList &pathsToTemplates
		, lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QString &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter
		, bool needInverting)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
	, TemplateParametrizedEntity(pathsToTemplates)
	, mNeedInverting(needInverting)
{
}

QString BoolPropertyConverter::convert(const QString &luaCode) const
{
	const QString actualCode = mNeedInverting ? QString("not(%1)").arg(luaCode) : luaCode;
	return mLuaTranslator.castTo(qrtext::core::wrap(new qrtext::lua::types::Boolean)
			, actualCode, mId, mPropertyName, mReservedVariablesConverter);
}
