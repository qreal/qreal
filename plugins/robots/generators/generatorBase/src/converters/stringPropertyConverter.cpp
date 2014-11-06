#include "stringPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

StringPropertyConverter::StringPropertyConverter(lua::LuaProcessor &luaTranslator
		, qReal::Id const &id
		, QString const &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
{
}
