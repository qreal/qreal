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

#include <QtCore/QString>
#include <QtCore/QPair>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrrepo/logicalRepoApi.h>

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForProperties
{
public:
	static QPair<QString, QString> countPropertyCharacteristicForConstraintElement(
			const qReal::Id &constraint
			, const QString &characteristicName
			, const QString &defaultValue
			, const QString &property
			, QString sign
			, QString value
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, qReal::ErrorReporterInterface &errorReporter);

	static QString generateExistsProperty(
			const QString &resElementName
			, const QString &elementName
			, const qReal::Id &constraint
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api);
};

}
}
}
