#include "boolPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

BoolPropertyConverter::BoolPropertyConverter(const QString &pathToTemplates
		, lua::LuaProcessor &luaTranslator
		, const qReal::Id &id
		, const QString &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter
		, bool needInverting)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
	, TemplateParametrizedEntity(pathToTemplates)
	, mNeedInverting(needInverting)
{
}

QString BoolPropertyConverter::convert(const QString &data) const
{
	const QString preparedCode = CodeConverterBase::convert(data);
	return mNeedInverting ? invert(preparedCode) : preparedCode;
}

QString BoolPropertyConverter::invert(const QString &expression) const
{
	QString invertTemplate = readTemplate("conditional/negation.t");
	invertTemplate.replace("@@CONDITION@@", expression);
	return invertTemplate;
}
