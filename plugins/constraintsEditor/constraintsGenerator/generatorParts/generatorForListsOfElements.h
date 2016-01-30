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

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForListsOfElements
{
public:
	static QPair<QString, QStringList > countConstraintForChildrens(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QStringList > countConstraintForOutgoingLinks(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QStringList > countConstraintForIncomingLinks(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QStringList > countConstraintForOutgoingNodes(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	static QPair<QString, QStringList > countConstraintForIncomingNodes(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

private:
	static QPair<QString, QStringList > countConstraintForListOfElements(
			const qReal::Id &constraint
			, const QString &elementName
			, const QString &resElementName
			, const QString &functionName
			, const QString &resType
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);
};

}
}
}
