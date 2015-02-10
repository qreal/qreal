#include "stringPropertyConverter.h"

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

StringPropertyConverter::StringPropertyConverter(lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QString &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
{
}

QString StringPropertyConverter::convert(const QString &expression) const
{
	return mLuaTranslator.castToString(expression, mId, mPropertyName, mReservedVariablesConverter);
}
