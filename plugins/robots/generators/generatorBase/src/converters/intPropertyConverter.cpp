#include "intPropertyConverter.h"

#include <qrtext/lua/luaToolbox.h>
#include <qrtext/lua/types/integer.h>

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

IntPropertyConverter::IntPropertyConverter(const QString &pathToTemplates
		, lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QString &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter
		, simple::Binding::ConverterInterface *typeConverter)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
	, TemplateParametrizedEntity(pathToTemplates)
	, mTypeConverter(typeConverter)
{
}

IntPropertyConverter::~IntPropertyConverter()
{
	delete mTypeConverter;
}

QString IntPropertyConverter::convert(const QString &data) const
{
	const QString preparedCode = CodeConverterBase::convert(data);
	if (preparedCode.isEmpty()) {
		return "0";
	}

	if (mLuaTranslator.toolbox().type(mLuaTranslator.toolbox()
			.parse(mId, QString(), data))->is<qrtext::lua::types::Integer>())
	{
		return preparedCode;
	}

	QString castTemplate = readTemplate("types/cast.t");
	const QString intType = mTypeConverter->convert("int");
	castTemplate.replace("@@TYPE@@", intType);
	castTemplate.replace("@@EXPRESSION@@", preparedCode);
	return castTemplate;
}

