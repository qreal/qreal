#include "functionBlockConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FunctionBlockConverter::FunctionBlockConverter(lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QString &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
{
}
