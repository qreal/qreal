#pragma once

#include "codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as a set of expressions in some 'function'
/// block. Splits it by ';' separator and joines resulting lines of code.
class FunctionBlockConverter : public CodeConverterBase
{
public:
	FunctionBlockConverter(QString const &pathToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, qrtext::LanguageToolboxInterface &textLanguage
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter
			, parts::DeviceVariables const &deviceVariables
	);

	virtual QString convert(QString const &data) const;
};

}
}
