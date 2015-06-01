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

OutputPortNameConverter::OutputPortNameConverter(QString const &pathToTemplates)
	: TemplateParametrizedConverter(pathToTemplates)
{
}

QString OutputPortNameConverter::convert(QString const &portNameOrAlias) const
{
	QString portName = portNameOrAlias;
	QStringList myStringList = portName.toUpper().split(QRegExp("\\W+"), QString::SkipEmptyParts);
	QSet<QString> stringSet = QSet<QString>::fromList(myStringList);
	portName = "";

	foreach (const QString &value, stringSet) {
		portName += value;
	}

	if (portName.isEmpty()) {
		return QString();
	}

	for (int i = 0; i < portName.length(); i++) {
		if (portName.at(i) != 'A' && portName.at(i) != 'B'
				&& portName.at(i) != 'C' && portName.at(i) != 'D') {
			return QString();
		}
	}

	QString const portTemplate = QString("ports/%1.t").arg(portName);
	return readTemplateIfExists(portTemplate, portName);
}
