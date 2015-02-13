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
