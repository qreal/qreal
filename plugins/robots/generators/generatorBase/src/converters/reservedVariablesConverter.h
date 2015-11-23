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

#pragma once

#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <kitBase/robotModel/robotModelInterface.h>

#include "generatorBase/converters/templateParametrizedConverter.h"
#include "generatorBase/parts/deviceVariables.h"

namespace generatorBase {
namespace converters {

/// Replaces the given reserved variable occurence to the corresponding expression
/// in the target language.
class ReservedVariablesConverter : public TemplateParametrizedConverter
{
public:
	ReservedVariablesConverter(const QStringList &pathsToTemplates
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelInterface &robotModel
			, const QMap<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> &devices
			, const simple::Binding::ConverterInterface *inputPortConverter
			, const parts::DeviceVariables &deviceVariables);

	~ReservedVariablesConverter() override;

	QString convert(const QString &variable) const override;

protected:
	virtual QString deviceExpression(const kitBase::robotModel::PortInfo &port) const;

	qReal::ErrorReporterInterface &mErrorReporter;
	const kitBase::robotModel::RobotModelInterface &mRobotModel;
	const QMap<kitBase::robotModel::PortInfo, kitBase::robotModel::DeviceInfo> mDevices;
	const simple::Binding::ConverterInterface *mInputConverter;  // Takes ownership
	const parts::DeviceVariables &mDeviceVariables;
};

}
}
