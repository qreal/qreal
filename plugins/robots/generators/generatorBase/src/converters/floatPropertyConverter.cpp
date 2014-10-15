#include "floatPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FloatPropertyConverter::FloatPropertyConverter(lua::LuaProcessor &luaTranslator
		, qReal::Id const &id
		, simple::Binding::ConverterInterface *reservedVariablesConverter)
	: CodeConverterBase(luaTranslator, id, reservedVariablesConverter)
{
}
