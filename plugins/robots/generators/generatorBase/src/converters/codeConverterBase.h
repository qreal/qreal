#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/deviceInfo.h>

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {
namespace converters {

/// A base class for all converters processing code. Processes input string
/// replacing sensor and encoder variables with code specified in templates from
/// "sensors" folder, and function invocations with code specified in templates
/// from "functions" folder
class CodeConverterBase : public TemplateParametrizedConverter
{
public:
	CodeConverterBase(QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *outputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter);

	virtual ~CodeConverterBase();

	/// Substitutes all nessesary system stuff linke sensor and encoder reading
	/// and funciton invocations code.
	virtual QString convert(QString const &data) const;

protected:
	/// Replaces all sensor and encoder variables occurences with corresponding
	/// nxtOSEK API expression.
	virtual QString replaceSystemVariables(QString const &expression) const;

	/// Replaces function invocations to calls of corresponding functions in C.
	virtual QString replaceFunctionInvocations(QString const &expression) const;

	virtual QString readSensorTemplatePath(interpreterBase::robotModel::DeviceInfo const &sensor) const;
	virtual QString sensorExpression(interpreterBase::robotModel::PortInfo const &port) const;
	virtual QString encoderExpression(QString const &port) const;
	virtual QString timelineExpression() const;

	simple::Binding::ConverterInterface const *mInputConverter;  // Takes ownership
	simple::Binding::ConverterInterface const *mOutputConverter;  // Takes ownership
	simple::Binding::ConverterInterface const *mFunctionInvocationsConverter;  // Takes ownership
};

}
}
