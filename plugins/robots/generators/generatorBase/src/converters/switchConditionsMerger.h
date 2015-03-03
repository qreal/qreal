#pragma once

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {
namespace converters {

/// Accepts an expression and a set of values and returns composite condition
/// of the equality of the given expression value to one of the given.
/// For example it can be done with a number of disjunctions or empty case expressions.
class SwitchConditionsMerger : public TemplateParametrizedConverter
{
public:
	SwitchConditionsMerger(const QString &pathToTemplates
			, const ConverterInterface * const systemVariablesConverter
			, const QStringList &values);
	~SwitchConditionsMerger();

	QString convert(const QString &expression) const override;

private:
	const ConverterInterface *mSystemVariablesConverter;  // Takes ownership
	const QStringList mValues;
};

}
}
