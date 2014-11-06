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
	SwitchConditionsMerger(QString const &pathToTemplates
			, ConverterInterface const * const systemVariablesConverter
			, QStringList const &values);
	~SwitchConditionsMerger();

	QString convert(QString const &expression) const override;

private:
	ConverterInterface const *mSystemVariablesConverter;  // Takes ownership
	QStringList const mValues;
};

}
}
