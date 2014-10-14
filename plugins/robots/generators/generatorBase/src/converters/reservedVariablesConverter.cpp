#include "reservedVariablesConverter.h"

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

ReservedVariablesConverter::ReservedVariablesConverter(QString const &pathToTemplates
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, parts::DeviceVariables const &deviceVariables)
	: TemplateParametrizedConverter(pathToTemplates)
	, mErrorReporter(errorReporter)
	, mRobotModel(robotModel)
	, mDevices(devices)
	, mInputConverter(inputPortConverter)
	, mDeviceVariables(deviceVariables)
{
}

ReservedVariablesConverter::~ReservedVariablesConverter()
{
	delete mInputConverter;
}

QString ReservedVariablesConverter::convert(QString const &variable) const
{
	for (interpreterBase::robotModel::PortInfo const &port : mRobotModel.availablePorts()) {
		QString const reservedVariable = port.reservedVariable();
		if (!reservedVariable.isEmpty() && variable == reservedVariable) {
			return deviceExpression(port);
		}
	}

	return variable;
}

QString ReservedVariablesConverter::deviceExpression(interpreterBase::robotModel::PortInfo const &port) const
{
	interpreterBase::robotModel::DeviceInfo const device = mDevices[port];
	if (device.isNull()) {
		mErrorReporter.addError(QObject::tr("Device on port %1 is not configured."\
				" Please select it on the left-side panel.").arg(port.name()));
		return QObject::tr("/* ERROR: SELECT DEVICE TYPE */");
	}

	QString const templatePath = mDeviceVariables.variableTemplatePath(device, port);

	// Converter must take a string like "1" or "2" (and etc) and return correct value
	return readTemplate(templatePath).replace("@@PORT@@", mInputConverter->convert(port.name()));
}
