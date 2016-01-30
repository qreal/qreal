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

#include "generatorForEdgeElements.h"
#include "generatorForElements.h"
#include "generatorForNodeElements.h"
#include "generatorForExpressions.h"
#include "generatorForLinks.h"

using namespace constraints::generator::generatorParts;
using namespace qReal;
using namespace qrRepo;

QString GeneratorForEdgeElements::countRealConstraintForEdgeElement(
		const Id &constraintElement
		, const QString &elementName
		, const QString &resultName
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	return GeneratorForElements::countRealConstraintForElement(
			constraintElement
			, edge
			, elementName
			, resultName
			, depth
			, additionalString
			, api
			, countsOfConstraintElementsInOneConstraint
			, errorReporter);
}

QPair<QString, QStringList> GeneratorForEdgeElements::countRealConstraintForOneEdgeElement(
		const Id &constraint
		, IdList &usedElements
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter
		, const bool isMultiOr)
{
	QString resString = "";
	QStringList resBool;
	QStringList allResultBool;

	QString constraintType = constraint.element();
	if (!countsOfConstraintElementsInOneConstraint.contains(constraintType)) {
		countsOfConstraintElementsInOneConstraint.insert(constraintType, 0);
	}

	int additionalDepth = countsOfConstraintElementsInOneConstraint[constraintType];

	QPair<QString, QStringList > nodeConstraint;
	if ((!GeneratorForLinks::linkWithGivenTypeExists(constraint, "MultiOrEdge", api)) || isMultiOr) {
		if (constraintType == "BeginNode") {
			nodeConstraint = GeneratorForNodeElements::countConstraintForBeginNode(
					constraint
					, elementName
					, depth + additionalDepth
					, additionalString
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter);
		} else if (constraintType == "EndNode") {
			nodeConstraint = GeneratorForNodeElements::countConstraintForEndNode(
					constraint
					, elementName
					, depth + additionalDepth
					, additionalString
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter);
		} else if (constraintType == "PropertyNode") {
			nodeConstraint = GeneratorForNodeElements::countConstraintForPropertyNode(
					constraint
					, elementName
					, depth + additionalDepth
					, additionalString
					, api
					, errorReporter);
		}
	} else if (constraintType == "MultiOrNode") {
		nodeConstraint = GeneratorForNodeElements::countConstraintForMultiOrNode(
				constraint
				, usedElements
				, edge
				, elementName
				, depth + additionalDepth
				, additionalString
				, api
				, errorReporter
				, countsOfConstraintElementsInOneConstraint);
	}

	resString += nodeConstraint.first;
	resBool.append(nodeConstraint.second);

	countsOfConstraintElementsInOneConstraint[constraintType]++;

	QPair<QString, QStringList > resNeighborsNodes = GeneratorForNodeElements::countNeighborsElementsByOr(
			constraint
			, GeneratorForExpressions::conjunctionExpression(resBool)
			, usedElements
			, edge
			, elementName
			, depth + additionalDepth
			, additionalString
			, api
			, countsOfConstraintElementsInOneConstraint
			, errorReporter);

	resString += resNeighborsNodes.first;

	if (!resNeighborsNodes.second.isEmpty()) {
		resBool.append(resNeighborsNodes.second);
	}

	allResultBool.append(GeneratorForExpressions::conjunctionExpression(resBool));

	return {resString, allResultBool};
}
