/* Copyright 2017 Grigory Zimin
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

class DynamicPropertiesConverter : public simple::Binding::ConverterInterface, public TemplateParametrizedEntity
{
public:
	DynamicPropertiesConverter(lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QStringList &pathsToTemplates
			, simple::Binding::ConverterInterface *reservedVariablesConverter
	);

	~DynamicPropertiesConverter() override;

	QString convert(const QString &properties) const override;
private:
	lua::LuaProcessor &mLuaTranslator;
	const qReal::Id mId;
	const QString mPropertyName;
	simple::Binding::ConverterInterface *mReservedVariablesConverter; // has ownership
};

}
}
