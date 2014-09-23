#include "switchConditionsMerger.h"

using namespace generatorBase::converters;

SwitchConditionsMerger::SwitchConditionsMerger(QString const &pathToTemplates
		, ConverterInterface const * const systemVariablesConverter
		, QStringList const &values)
	: TemplateParametrizedConverter(pathToTemplates)
	, mSystemVariablesConverter(systemVariablesConverter)
	, mValues(values)
{
}

SwitchConditionsMerger::~SwitchConditionsMerger()
{
	delete mSystemVariablesConverter;
}

QString SwitchConditionsMerger::convert(QString const &expression) const
{
	QString const convertedExpression = mSystemVariablesConverter->convert(expression);
	QString const oneCondition = readTemplate("switch/oneCase.t");
	QString const conditionsSeparator = readTemplate("switch/conditionsSeparator.t");

	QStringList conditions;
	for (QString const &value : mValues) {
		QString condition = oneCondition;
		conditions << condition.replace("@@EXPRESSION@@", convertedExpression).replace("@@VALUE@@", value);
	}

	return conditions.join(conditionsSeparator);
}
