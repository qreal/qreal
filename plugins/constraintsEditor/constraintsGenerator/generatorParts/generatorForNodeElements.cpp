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

#include "generatorForNodeElements.h"
#include "generatorForElements.h"
#include "generatorForProperties.h"
#include "generatorForListsOfElements.h"
#include "generatorForEdgeElements.h"
#include "generatorForExpressions.h"
#include "generatorForLinks.h"

using namespace constraints::generator::generatorParts;
using namespace qReal;
using namespace qrRepo;

QString GeneratorForNodeElements::countRealConstraintForNodeElement(const Id &constraintElement
		, const QString &elementName
		, const QString &resultName
		, const int depth
		, const QString &addStr
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	return GeneratorForElements::countRealConstraintForElement(
			constraintElement, node, elementName, resultName, depth, addStr, api
			, countsOfConstraintElementsInOneConstraint, errorReporter);
}

QPair<QString, QStringList> GeneratorForNodeElements::countRealConstraintForOneNodeElement(
		const Id &constraint
		, IdList &usedElements
		, const QString &elementName
		, const int depth
		, const QString &addStr
		, const LogicalRepoApi &api
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

	QPair<QString, QStringList> listConstraint;
	if ((!GeneratorForLinks::linkWithGivenTypeExists(constraint, "MultiOrEdge", api)) || isMultiOr) {
		if (constraintType == "Parent") {
			listConstraint = countConstraintForParent(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter);
		} else if (constraintType == "Childrens") {
			listConstraint = GeneratorForListsOfElements::countConstraintForChildrens(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);
		} else if (constraintType == "OutgoingLinks") {
			listConstraint = GeneratorForListsOfElements::countConstraintForOutgoingLinks(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);
		} else if (constraintType == "IncomingLinks") {
			listConstraint = GeneratorForListsOfElements::countConstraintForIncomingLinks(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);
		} else if (constraintType == "OutgoingNodes") {
			listConstraint = GeneratorForListsOfElements::countConstraintForOutgoingNodes(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);
		} else if (constraintType == "IncomingNodes") {
			listConstraint = GeneratorForListsOfElements::countConstraintForIncomingNodes(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);
		} else if (constraintType == "PropertyNode") {
			listConstraint = countConstraintForPropertyNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter);
		}
	} else if (constraintType == "MultiOrNode") {
		listConstraint = countConstraintForMultiOrNode(
				constraint
				, usedElements
				, node
				, elementName
				, depth + additionalDepth
				, addStr
				, api
				, errorReporter
				, countsOfConstraintElementsInOneConstraint);
	}

	resString += listConstraint.first;
	resBool.append(listConstraint.second);

	countsOfConstraintElementsInOneConstraint[constraintType]++;

	QPair<QString, QStringList> resNeighborsNodes = countNeighborsElementsByOr(
			constraint
			, GeneratorForExpressions::conjunctionExpression(resBool)
			, usedElements
			, node, elementName
			, depth + additionalDepth
			, addStr
			, api
			, countsOfConstraintElementsInOneConstraint
			, errorReporter);

	resString += resNeighborsNodes.first;

	if (!resNeighborsNodes.second.isEmpty()) {
		resBool = resNeighborsNodes.second;
	}

	allResultBool.append(GeneratorForExpressions::conjunctionExpression(resBool));

	return QPair<QString, QStringList>(resString, allResultBool);
}

QPair<QString, QStringList> GeneratorForNodeElements::countConstraintForBeginNode(const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	QString resultString = "";
	QStringList resBool;
	QStringList allResultBool;

	resultString += QString("%1qReal::Id newBeginNodeName_%2 = logicalApi.from(%3);\n")
			.arg(additionalString)
			.arg(depth)
			.arg(elementName);

	resultString += countRealConstraintForNodeElement(
			constraint
			, "newBeginNodeName_" + QString::number(depth)
			, "beginNodeRes", depth + 1
			, additionalString
			, api
			, countsOfConstraintElementsInOneConstraint
			, errorReporter);

	resBool.push_back("beginNodeRes_" + QString::number(depth + 1));

	resultString += GeneratorForProperties::generateExistsProperty("beginNodeRes"
			, "newBeginNodeName"
			, constraint
			, depth
			, additionalString
			, api);

	resBool.push_back("beginNodeRes_" + QString::number(depth));

	allResultBool.append(GeneratorForExpressions::conjunctionExpression(resBool));
	return {resultString, allResultBool};
}

QPair<QString, QStringList> GeneratorForNodeElements::countConstraintForEndNode(const Id &constraint
		, const QString &elementName
		, int depth
		, const QString &additionalString
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	QString resultString = "";
	QStringList resBool;
	QStringList allResultBool;

	resultString += QString("%1qReal::Id newEndNodeName_%2 = logicalApi.to(%3);\n")
			.arg(additionalString)
			.arg(QString::number(depth))
			.arg(elementName);

	resultString += countRealConstraintForNodeElement(
			constraint
			, "newEndNodeName_" + QString::number(depth)
			, "endNodeRes"
			, depth + 1
			, additionalString
			, api
			, countsOfConstraintElementsInOneConstraint
			, errorReporter
			);

	resBool.push_back("endNodeRes_" + QString::number(depth + 1));

	resultString += GeneratorForProperties::generateExistsProperty("endNodeRes"
			, "newEndNodeName"
			, constraint
			, depth
			, additionalString
			, api);

	resBool.push_back("endNodeRes_" + QString::number(depth));

	allResultBool.append(GeneratorForExpressions::conjunctionExpression(resBool));
	return QPair<QString, QStringList >(resultString, allResultBool);
}

QPair<QString, QStringList> GeneratorForNodeElements::countConstraintForParent(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	QString resultString = "";
	QStringList resultBool;
	QStringList allResultBool;

	resultString += QString("%1qReal::Id newParentName_%2 = logicalApi.parent(%3);\n"
			"%1bool mainParentRes_%2 = true;\n"
			"%1if (newParentName_%2 != qReal::Id::rootId()) {\n")
			.arg(additionalString)
			.arg(depth)
			.arg(elementName);

	const QString realConstraintForNodeElement = countRealConstraintForNodeElement(
			constraint
			, "newParentName_" + QString::number(depth)
			, "parentNodeRes"
			, depth + 1
			, additionalString + "	"
			, api
			, countsOfConstraintElementsInOneConstraint
			, errorReporter);

	resultString += additionalString + realConstraintForNodeElement;

	resultString += QString("%1	mainParentRes_%2 = parentNodeRes_%3;\n"
						 "%1	}\n")
			.arg(additionalString)
			.arg(depth)
			.arg(depth + 1);

	resultBool.push_back("mainParentRes_" + QString::number(depth));

	resultString += GeneratorForProperties::generateExistsProperty(
			"parentRes"
			, "newParentName"
			, constraint
			, depth
			, additionalString
			, api);

	resultBool.push_back("parentRes_" + QString::number(depth));

	allResultBool.append(GeneratorForExpressions::conjunctionExpression(resultBool));

	return QPair<QString, QStringList >(resultString, allResultBool);
}

QPair<QString, QStringList> GeneratorForNodeElements::countConstraintForPropertyNode(const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &addStr
		, const LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter)
{
	const QString property = api.property(constraint, "property").toString();
	const QString sign = api.property(constraint, "sign").toString();
	const QString value = api.property(constraint, "value").toString();

	QPair<QString, QString> res = GeneratorForProperties::countPropertyCharacteristicForConstraintElement(
			constraint
			, "propertyNode"
			, "true"
			, property
			, sign
			, value
			, elementName
			, depth
			, addStr
			, errorReporter);

	QStringList resBool;
	resBool.push_back(res.second);

	return QPair<QString, QStringList >(res.first, resBool);
}

QPair<QString, QStringList> GeneratorForNodeElements::countConstraintForMultiOrNode(
		const Id &constraint
		, IdList &usedElements
		, const metaType &type
		, const QString &elementName
		, const int depth
		, const QString &addStr
		, const LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint)
{
	QString resString = "";
	QStringList curBool;
	QStringList resBool;

	IdList neighborNodes = neighborNodesWithGivenType(constraint, "MultiOrEdge", api);
	usedElements.append(neighborNodes);

	for (const Id element : neighborNodes) {
		QPair<QString, QStringList > resElementConstraint;
		if (type == edge) {
			resElementConstraint = GeneratorForEdgeElements::countRealConstraintForOneEdgeElement(
						element
						, usedElements
						, elementName
						, depth + 1
						, addStr
						, api
						, countsOfConstraintElementsInOneConstraint
						, errorReporter
						, true);
		} else if (type == node) {
			resElementConstraint = countRealConstraintForOneNodeElement(
						element
						, usedElements
						, elementName
						, depth + 1
						, addStr
						, api
						, countsOfConstraintElementsInOneConstraint
						, errorReporter
						, true);
		}

		resString += resElementConstraint.first;
		curBool.append(resElementConstraint.second);
	}

	resBool.append(QString("(" + GeneratorForExpressions::disjunctionExpression(curBool) + ")"));

	return QPair<QString, QStringList >(resString, resBool);
}

QPair<QString, QStringList> GeneratorForNodeElements::countNeighborsElementsByOr(
		const Id &constraint
		, const QString &resConstraintBool
		, IdList &usedElements
		, const metaType &type
		, const QString &elementName
		, const int depth
		, const QString &addStr
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	QString resString = "";
	QStringList resBool;

	IdList neighborNodes = neighborNodesWithGivenType(constraint, "Or", api);

	if (!neighborNodes.isEmpty()) {
		for (const Id neighbor : neighborNodes) {
			if (!usedElements.contains(neighbor)) {
				usedElements.append(neighbor);
				QPair<QString, QStringList > resNeighborsConstraint;

				if (type == edge) {
					resNeighborsConstraint = GeneratorForEdgeElements::countRealConstraintForOneEdgeElement(
							neighbor
							, usedElements
							, elementName
							, depth
							, addStr
							, api
							, countsOfConstraintElementsInOneConstraint
							, errorReporter
							);
				} else if (type == node) {
					resNeighborsConstraint = countRealConstraintForOneNodeElement(
							neighbor
							, usedElements
							, elementName
							, depth
							, addStr
							, api
							, countsOfConstraintElementsInOneConstraint
							, errorReporter);
				}

				resString += resNeighborsConstraint.first;
				resBool.append(QString("( %1 || %2 )")
						.arg(resConstraintBool)
						.arg(GeneratorForExpressions::conjunctionExpression(resNeighborsConstraint.second)));
			}
		}
	}

	return QPair<QString, QStringList >(resString, resBool);
}

IdList GeneratorForNodeElements::neighborNodesWithGivenType(
		const Id &element
		, const QString &type
		, const LogicalRepoApi &api)
{
	IdList inNodes;

	for (const Id &inLink : api.incomingLinks(element)) {
		if (inLink.element() == type) {
			inNodes.push_back(api.from(inLink));
		}
	}

	IdList outNodes;
	for (Id const &outLink : api.outgoingLinks(element)) {
		if (outLink.element() == type) {
			outNodes.push_back(api.to(outLink));
		}
	}

	return inNodes << outNodes;
}
