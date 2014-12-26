#include "intPropertyConverter.h"

#include <qrtext/lua/luaToolbox.h>
#include <qrtext/lua/types/integer.h>

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

IntPropertyConverter::IntPropertyConverter(QString const &pathToTemplates
		, lua::LuaProcessor &luaTranslator
		, qReal::Id const &id
		, QString const &propertyName
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

QString IntPropertyConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);
	if (preparedCode.isEmpty()) {
		return "0";
	}

	if (mLuaTranslator.toolbox().type(mLuaTranslator.toolbox()
			.parse(mId, QString(), data))->is<qrtext::lua::types::Integer>())
	{
		return preparedCode;
	}

	QString castTemplate = readTemplate("types/cast.t");
	QString const intType = mTypeConverter->convert("int");
	castTemplate.replace("@@TYPE@@", intType);
	castTemplate.replace("@@EXPRESSION@@", preparedCode);
	return castTemplate;
}

