/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "reservedVariablesConverter.h"

#include "generatorBase/lua/luaProcessor.h"

using namespace generatorBase::converters;
using namespace qReal;

ReservedVariablesConverter::ReservedVariablesConverter(const QStringList &pathsToTemplates
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelInterface &robotModel
		, QMap<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> const &devices
		, const simple::Binding::ConverterInterface *inputPortConverter
		, const parts::DeviceVariables &deviceVariables)
	: TemplateParametrizedConverter(pathsToTemplates)
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

QString ReservedVariablesConverter::convert(const QString &variable) const
{
	for (const kitBase::robotModel::PortInfo &port : mRobotModel.availablePorts()) {
		const QString reservedVariable = port.reservedVariable();
		if (!reservedVariable.isEmpty() && variable == reservedVariable) {
			return deviceExpression(port);
		}
	}

	for (const QString &button : mRobotModel.buttonCodes().keys()) {
		if (variable == button) {
			return readTemplate(QString("buttons/%1.t").arg(button));
		}
	}

	return variable;
}

QString ReservedVariablesConverter::deviceExpression(const kitBase::robotModel::PortInfo &port) const
{
	const kitBase::robotModel::DeviceInfo device = mDevices[port];
	if (device.isNull()) {
		mErrorReporter.addError(QObject::tr("Device on port %1 is not configured."\
				" Please select it on the \"Configure devices\" panel on the right-hand side.")
						.arg(port.userFriendlyName())
		);
		return QObject::tr("/* ERROR: SELECT DEVICE TYPE */");
	}

	const QString templatePath = mDeviceVariables.variableTemplatePath(device, port);

	// Converter must take a string like "1" or "2" (and etc) and return correct value
	return readTemplate(templatePath).replace("@@PORT@@", mInputConverter->convert(port.name()));
}
