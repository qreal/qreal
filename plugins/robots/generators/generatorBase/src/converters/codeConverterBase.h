#pragma once

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {

namespace lua {
class LuaProcessor;
}

namespace converters {

/// A base class for all converters processing code. Prints the given lua code on the
/// target language replacing reserved variables with code specified in templates from
/// "sensors" folder, and function invocations with code specified in templates from
/// "functions" folder.
class CodeConverterBase : public simple::Binding::ConverterInterface
{
public:
	/// Takes ownership on @arg reservedVariablesConverter.
	CodeConverterBase(lua::LuaProcessor &luaTranslator
			, qReal::Id const &id
			, QString const &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);

	QString convert(QString const &luaCode) const override;

protected:
	lua::LuaProcessor &mLuaTranslator;
	qReal::Id const mId;
	QString const mPropertyName;
	simple::Binding::ConverterInterface const *mReservedVariablesConverter;  // LuaProcessor will take ownership;
};

}
}
