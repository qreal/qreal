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

OutputPortNameConverter::OutputPortNameConverter(const QStringList &pathsToTemplates)
	: TemplateParametrizedConverter(pathsToTemplates)
{
}

QString OutputPortNameConverter::convert(const QString &portNameOrAlias) const
{
	QString result;
	const QStringList ports = {"A", "B", "C", "D"};
	for (const QString &port : ports) {
		if (portNameOrAlias.contains(port)) {
			result += port;
		}
	}

	const QString portTemplate = QString("ports/%1.t").arg(result);
	return readTemplateIfExists(portTemplate, result);
}
