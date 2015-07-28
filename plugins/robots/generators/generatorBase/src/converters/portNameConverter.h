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

#include <kitBase/robotModel/portInfo.h>

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace generatorBase {
namespace converters {

/// Converts an output port with given id into generator-dependent port representation.
/// If port alias obtained it will be converted into the original port name.
/// Then port name will be tried to convert into the language-specific represenation.
/// For this purpose converter will search for "ports/*.t" template where
/// * is a port name. If such template found its contents will be returned otherwise
/// the result will be raw port name.
class PortNameConverter : public TemplateParametrizedConverter
{
public:
	PortNameConverter(const QStringList &pathsToTemplates
			, const QList<kitBase::robotModel::PortInfo> &ports
			, qReal::ErrorReporterInterface &errorReporter);

	QString convert(const QString &portNameOrAlias) const override;

private:
	const QList<kitBase::robotModel::PortInfo> mPorts;
	qReal::ErrorReporterInterface &mErrorReporter;
};

}
}
