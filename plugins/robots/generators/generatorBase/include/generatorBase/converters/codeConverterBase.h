#pragma once

#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {
namespace converters {

/// A base class for all converters processing code. Processes input string
/// replacing sensor and encoder variables with code specified in templates from
/// "sensors" folder, and function invocations with code specified in templates
/// from "functions" folder
class ROBOTS_GENERATOR_EXPORT CodeConverterBase : public TemplateParametrizedConverter
{
public:
	CodeConverterBase(QString const &pathToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, simple::Binding::ConverterInterface const *inputPortConverter
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

	virtual QString deviceExpression(interpreterBase::robotModel::PortInfo const &port) const;
	virtual QString timelineExpression() const;

	qReal::ErrorReporterInterface &mErrorReporter;
	interpreterBase::robotModel::RobotModelInterface const &mRobotModel;
	QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const mDevices;
	simple::Binding::ConverterInterface const *mInputConverter;  // Takes ownership
	simple::Binding::ConverterInterface const *mFunctionInvocationsConverter;  // Takes ownership
};

}
}
