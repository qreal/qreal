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

#pragma once

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {

namespace lua {
class LuaProcessor;
}

namespace converters {

/// A base class for all converters processing code. Prints the given lua code to the
/// target language replacing reserved variables with code specified in templates from
/// "sensors" folder, and function invocations with code specified in templates from
/// "functions" folder.
class CodeConverterBase : public simple::Binding::ConverterInterface
{
public:
	/// Takes ownership on @arg reservedVariablesConverter.
	CodeConverterBase(lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QString &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);

	QString convert(const QString &luaCode) const override;

protected:
	lua::LuaProcessor &mLuaTranslator;
	const qReal::Id mId;
	const QString mPropertyName;
	const simple::Binding::ConverterInterface *mReservedVariablesConverter;  // LuaProcessor will take ownership;
};

}
}
