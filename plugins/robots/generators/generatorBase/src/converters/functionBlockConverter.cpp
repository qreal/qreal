#include "functionBlockConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FunctionBlockConverter::FunctionBlockConverter(lua::LuaProcessor &luaTranslator
		, qReal::Id const &id
		, simple::Binding::ConverterInterface *reservedVariablesConverter)
	: CodeConverterBase(luaTranslator, id, reservedVariablesConverter)
{
}
