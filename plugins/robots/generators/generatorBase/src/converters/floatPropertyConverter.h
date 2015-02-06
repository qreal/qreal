#pragma once

#include "codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the float type.
class FloatPropertyConverter : public CodeConverterBase
{
public:
	FloatPropertyConverter(lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QString &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);
};

}
}
