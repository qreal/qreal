#pragma once

#include "codeConverterBase.h"
#include "generatorBase/parts/variables.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the int type.
/// Casts this expression to int type if type inferer hints returns other one.
/// Casting code must be specified in "types/cast.t" template.
class IntPropertyConverter : public CodeConverterBase
{
public:
	IntPropertyConverter(QString const &pathToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter
			, parts::DeviceVariables const &deviceVariables
			, simple::Binding::ConverterInterface const *typeConverter
			, parts::Variables const *variables);

	virtual ~IntPropertyConverter();

	virtual QString convert(QString const &data) const;

private:
	simple::Binding::ConverterInterface const *mTypeConverter;  // Takes ownership
	parts::Variables const *mVariables;  // Does not take ownership
};

}
}
