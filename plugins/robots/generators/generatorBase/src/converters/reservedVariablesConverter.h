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
	ReservedVariablesConverter(const QString &pathToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, const interpreterBase::robotModel::RobotModelInterface &robotModel
			, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
			, const simple::Binding::ConverterInterface *inputPortConverter
			, const parts::DeviceVariables &deviceVariables);

	~ReservedVariablesConverter() override;

	QString convert(const QString &variable) const override;

protected:
	virtual QString deviceExpression(const interpreterBase::robotModel::PortInfo &port) const;

	qReal::ErrorReporterInterface &mErrorReporter;
	const interpreterBase::robotModel::RobotModelInterface &mRobotModel;
	QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const mDevices;
	const simple::Binding::ConverterInterface *mInputConverter;  // Takes ownership
	const parts::DeviceVariables &mDeviceVariables;
};

}
}
