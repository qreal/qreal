#pragma once

#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "generatorBase/converters/templateParametrizedConverter.h"
#include "generatorBase/parts/deviceVariables.h"

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
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, simple::Binding::ConverterInterface const *inputPortConverter
			, simple::Binding::ConverterInterface const *functionInvocationsConverter
			, parts::DeviceVariables const &deviceVariables);

	~CodeConverterBase() override;

	/// Substitutes all nessesary system stuff linke sensor and encoder reading
	/// and funciton invocations code.
	QString convert(QString const &data) const override;

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
	parts::DeviceVariables const &mDeviceVariables;
};

}
}
