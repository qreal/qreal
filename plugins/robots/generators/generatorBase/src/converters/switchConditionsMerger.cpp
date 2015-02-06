#include "switchConditionsMerger.h"

using namespace generatorBase::converters;

SwitchConditionsMerger::SwitchConditionsMerger(const QString &pathToTemplates
		, const ConverterInterface * const systemVariablesConverter
		, const QStringList &values)
	: TemplateParametrizedConverter(pathToTemplates)
	, mSystemVariablesConverter(systemVariablesConverter)
	, mValues(values)
{
}

SwitchConditionsMerger::~SwitchConditionsMerger()
{
	delete mSystemVariablesConverter;
}

QString SwitchConditionsMerger::convert(const QString &expression) const
{
	const QString convertedExpression = mSystemVariablesConverter->convert(expression);
	const QString oneCondition = readTemplate("switch/oneCase.t");
	const QString conditionsSeparator = readTemplate("switch/conditionsSeparator.t");

	QStringList conditions;
	for (const QString &value : mValues) {
		QString condition = oneCondition;
		conditions << condition.replace("@@EXPRESSION@@", convertedExpression).replace("@@VALUE@@", value);
	}

	return conditions.join(conditionsSeparator);
}
