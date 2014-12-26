#pragma once

#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "generatorBase/converters/templateParametrizedConverter.h"
#include "generatorBase/parts/deviceVariables.h"

namespace generatorBase {
namespace converters {

/// Replaces the given reserved variable occurence to the corresponding expression
/// in the target language.
class ReservedVariablesConverter : public TemplateParametrizedConverter
{
public:
	ReservedVariablesConverter(QString const &pathToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, simple::Binding::ConverterInterface const *inputPortConverter
			, parts::DeviceVariables const &deviceVariables);

	~ReservedVariablesConverter() override;

	QString convert(QString const &variable) const override;

protected:
	virtual QString deviceExpression(interpreterBase::robotModel::PortInfo const &port) const;

	qReal::ErrorReporterInterface &mErrorReporter;
	interpreterBase::robotModel::RobotModelInterface const &mRobotModel;
	QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const mDevices;
	simple::Binding::ConverterInterface const *mInputConverter;  // Takes ownership
	parts::DeviceVariables const &mDeviceVariables;
};

}
}
