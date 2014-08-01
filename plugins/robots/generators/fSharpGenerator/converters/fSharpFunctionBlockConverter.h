#pragma once

#include "generatorBase/converters/codeConverterBase.h"

namespace fSharp {
namespace converters {

/// Processes input string treating it as a set of expressions in some 'function'
/// block. Splits it by ';' separator and joines resulting lines of code.
class FSharpFunctionBlockConverter : public generatorBase::converters::CodeConverterBase
{
public:
	FSharpFunctionBlockConverter(QString const &pathToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, generatorBase::simple::Binding::ConverterInterface const *inputPortConverter
			, generatorBase::simple::Binding::ConverterInterface const *functionInvocationsConverter
			, generatorBase::parts::DeviceVariables const &deviceVariables);

	virtual QString convert(QString const &data) const;
};

}
}
