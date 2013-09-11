#pragma once

#include "templateParametrizedConverter.h"
#include "../../../robotsInterpreter/sensorConstants.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

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
	virtual QString replaceSensorAndEncoderVariables(QString const &expression) const;

	/// Replaces function invocations to calls of corresponding functions in C.
	virtual QString replaceFunctionInvocations(QString const &expression) const;

	virtual QString readSensorTemplatePath(
			qReal::interpreters::robots::enums::sensorType::SensorTypeEnum sensorType) const;
	virtual QString sensorExpression(int port) const;
	virtual QString encoderExpression(QString const &port) const;

	simple::Binding::ConverterInterface const *mInputConverter;  // Takes ownership
	simple::Binding::ConverterInterface const *mOutputConverter;  // Takes ownership
	simple::Binding::ConverterInterface const *mFunctionInvocationsConverter;  // Takes ownership
};

}
}
}
}
