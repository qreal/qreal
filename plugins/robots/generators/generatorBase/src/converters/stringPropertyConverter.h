#pragma once

#include "codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Converts a given expression into an expression of the string type on the target language.
class StringPropertyConverter : public converters::CodeConverterBase
{
public:
	StringPropertyConverter(lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QString &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter);

	QString convert(const QString &expression) const override;
};

}
}
