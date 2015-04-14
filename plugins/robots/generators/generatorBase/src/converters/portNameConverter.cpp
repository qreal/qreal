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

#include "portNameConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

PortNameConverter::PortNameConverter(const QString &pathToTemplates
		, QList<kitBase::robotModel::PortInfo> const &ports)
	: TemplateParametrizedConverter(pathToTemplates)
	, mPorts(ports)
{
}

QString PortNameConverter::convert(const QString &portNameOrAlias) const
{
	QString portName;
	for (const kitBase::robotModel::PortInfo &port : mPorts) {
		if (port.name() == portNameOrAlias || port.nameAliases().contains(portNameOrAlias)) {
			portName = port.name();
			if (port.name() == portNameOrAlias) {
				// Port name is first-priority cause, for example, M1 in TRIK may be an alias for B1.
				break;
			}
		}
	}

	if (portName.isEmpty()) {
		/// @todo: Report an error somewhere
		return QString();
	}

	const QString portTemplate = QString("ports/%1.t").arg(portName);
	return readTemplateIfExists(portTemplate, portName);
}
