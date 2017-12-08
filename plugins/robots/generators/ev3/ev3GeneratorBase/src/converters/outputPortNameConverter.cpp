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

#include "outputPortNameConverter.h"

using namespace ev3::converters;

OutputPortNameConverter::OutputPortNameConverter(const QStringList &pathsToTemplates
		, const QList<kitBase::robotModel::PortInfo> &ports)
	: TemplateParametrizedConverter(pathsToTemplates)
	, mPorts(ports)
{
}

QString OutputPortNameConverter::convert(const QString &portNameOrAlias) const
{
	QString result;
	for (const kitBase::robotModel::PortInfo &port : mPorts) {
		if (port.direction() != kitBase::robotModel::output) {
			continue;
		}

		if (portNameOrAlias.contains(port.name())) {
			result += port.name();
		} else {
			for (const QString &alias : port.nameAliases()) {
				if (portNameOrAlias.contains(alias)) {
					result += port.name();
					break;
				}
			}
		}
	}

	const QString portTemplate = QString("ports/%1.t").arg(result);
	return readTemplateIfExists(portTemplate, result);
}
