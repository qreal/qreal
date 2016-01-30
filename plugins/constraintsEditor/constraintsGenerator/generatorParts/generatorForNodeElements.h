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

#include <qrkernel/ids.h>
#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorUtils/defs.h"

namespace constraints {
namespace generator {
namespace generatorParts {

class GeneratorForNodeElements
{
public:
	static QString countRealConstraintForNodeElement(const qReal::Id &constraintElement
			, const QString &elementName
			, const QString &resultName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			);

	static QPair<QString, QStringList> countRealConstraintForOneNodeElement(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const QString &elementName
			, const int depth
			, const QString &additionalString, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			, const bool isMultiOr = false
			);

	static QPair<QString, QStringList> countConstraintForBeginNode(qReal::Id const &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter
			);

	static QPair<QString, QStringList > countConstraintForEndNode(
			qReal::Id const &constraint
			, const QString &elementName
			, int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter);

	static QPair<QString, QStringList > countConstraintForParent(
			const qReal::Id &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter);

	static QPair<QString, QStringList > countConstraintForPropertyNode(qReal::Id const &constraint
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			);

	static QPair<QString, QStringList > countConstraintForMultiOrNode(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, const metaType &type
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, qReal::ErrorReporterInterface &errorReporter
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint);

	/// TODO: this method has to be somewhere else (used in nodeElements and edgeElements generator)
	static QPair<QString, QStringList > countNeighborsElementsByOr(
			const qReal::Id &constraint
			, const QString &resConstraintBool
			, qReal::IdList &usedElements
			, const metaType &type
			, const QString &elementName
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api
			, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
			, qReal::ErrorReporterInterface &errorReporter);

private:
	/// Returns list of neighbor nodes for given element with given type.
	/// @param element - element id.
	/// @param type - type of node.
	/// @param api - information about model.
	/// @returns list of neighbor nodes ids.
	static qReal::IdList neighborNodesWithGivenType(
			const qReal::Id &element
			, const QString &type
			, const qrRepo::LogicalRepoApi &api);
};

}
}
}
