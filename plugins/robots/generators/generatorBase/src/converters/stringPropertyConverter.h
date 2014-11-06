#pragma once

#include "codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Converts a given expression into an expression of the string type on the target language.
class StringPropertyConverter : public converters::CodeConverterBase
{
public:
	StringPropertyConverter(lua::LuaProcessor &luaTranslator
			, qReal::Id const &id
			, QString const &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);
};

}
}
