#pragma once

#include "codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as a set of expressions in some 'function'
/// block. Splits it by ';' separator and joins resulting lines of code.
class FunctionBlockConverter : public CodeConverterBase
{
public:
	FunctionBlockConverter(lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QString &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);
};

}
}
