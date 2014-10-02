
#include "intPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

IntPropertyConverter::IntPropertyConverter(QString const &pathToTemplates
		, lua::LuaProcessor &luaTranslator
		, qReal::Id const &id
		, simple::Binding::ConverterInterface *reservedVariablesConverter
		, simple::Binding::ConverterInterface *typeConverter)
	: CodeConverterBase(luaTranslator, id, reservedVariablesConverter)
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

//	enums::variableType::VariableType const expressionType = mVariables->expressionType(preparedCode);
//	if (expressionType == enums::variableType::intType) {
//		return preparedCode;
//	}

	QString castTemplate = readTemplate("types/cast.t");
	QString const intType = mTypeConverter->convert("int");
	castTemplate.replace("@@TYPE@@", intType);
	castTemplate.replace("@@EXPRESSION@@", preparedCode);
	return castTemplate;
}

