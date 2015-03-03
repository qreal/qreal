#include "codeConverterBase.h"

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

CodeConverterBase::CodeConverterBase(lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QString &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter)
	: mLuaTranslator(luaTranslator)
	, mId(id)
	, mPropertyName(propertyName)
	, mReservedVariablesConverter(reservedVariablesConverter)
{
}

QString CodeConverterBase::convert(const QString &luaCode) const
{
	return mLuaTranslator.translate(luaCode, mId, mPropertyName, mReservedVariablesConverter);
}
