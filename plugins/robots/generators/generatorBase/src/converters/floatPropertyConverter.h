#pragma once

#include "codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the float type.
class FloatPropertyConverter : public CodeConverterBase
{
public:
	FloatPropertyConverter(lua::LuaProcessor &luaTranslator
			, qReal::Id const &id
			, QString const &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);
};

}
}
