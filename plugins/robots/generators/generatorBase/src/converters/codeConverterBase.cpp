#include "codeConverterBase.h"

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

CodeConverterBase::CodeConverterBase(lua::LuaProcessor &luaTranslator
		, qReal::Id const &id
		, simple::Binding::ConverterInterface *reservedVariablesConverter)
	: mLuaTranslator(luaTranslator)
	, mId(id)
	, mReservedVariablesConverter(reservedVariablesConverter)
{
}

QString CodeConverterBase::convert(QString const &luaCode) const
{
	return mLuaTranslator.translate(luaCode, mId, mReservedVariablesConverter);
}
