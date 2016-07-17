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

#include "codeConverterBase.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/parts/variables.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the int type.
/// Casts this expression to int type if type inferer hints returns other one.
/// Casting code must be specified in "types/cast.t" template.
class IntPropertyConverter : public CodeConverterBase, public TemplateParametrizedEntity
{
public:
	IntPropertyConverter(const QStringList &pathsToTemplates
			, lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QString &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);

	QString convert(const QString &luaCode) const override;
};

}
}
