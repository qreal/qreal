#pragma once

#include "generatorBase/converters/codeConverterBase.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the float type.
class FloatPropertyConverter : public CodeConverterBase
{
public:
	FloatPropertyConverter(QString const &pathToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter);
};

}
}
