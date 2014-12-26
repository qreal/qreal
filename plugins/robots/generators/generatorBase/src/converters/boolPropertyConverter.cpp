#include "boolPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

BoolPropertyConverter::BoolPropertyConverter(QString const &pathToTemplates
		, lua::LuaProcessor &luaTranslator
		, qReal::Id const &id
		, QString const &propertyName
		, simple::Binding::ConverterInterface *reservedVariablesConverter
		, bool needInverting)
	: CodeConverterBase(luaTranslator, id, propertyName, reservedVariablesConverter)
	, TemplateParametrizedEntity(pathToTemplates)
	, mNeedInverting(needInverting)
{
}

QString BoolPropertyConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);
	return mNeedInverting ? invert(preparedCode) : preparedCode;
}

QString BoolPropertyConverter::invert(QString const &expression) const
{
	QString invertTemplate = readTemplate("conditional/negation.t");
	invertTemplate.replace("@@CONDITION@@", expression);
	return invertTemplate;
}
