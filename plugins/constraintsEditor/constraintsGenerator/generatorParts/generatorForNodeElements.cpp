#include "generatorForNodeElements.h"
#include "generatorForElements.h"
#include "generatorForProperties.h"
#include "generatorUtils/auxiliaryMethods.h"
#include "generatorForListsOfElements.h"
#include "generatorForEdgeElements.h"

using namespace constraints::generator::generatorParts;
using namespace constraints::generator::generatorUtils;
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

QPair<QString, QList<QString>> GeneratorForNodeElements::countRealConstraintForOneNodeElement(
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
	QList<QString> resBool;
	QList<QString> allResultBool;

	QString constraintType = constraint.element();

	if (!countsOfConstraintElementsInOneConstraint.contains(constraintType)) {
		countsOfConstraintElementsInOneConstraint.insert(constraintType, 0);
	}

	int additionalDepth = countsOfConstraintElementsInOneConstraint[constraintType];

	if ((!AuxiliaryMethods::linkWithGivenTypeExists(constraint, "MultiOrEdge", api)) || isMultiOr) {
		if (constraintType == "Parent") {
			QPair<QString, QList<QString> > resParentConstraint =
					countConstraintForParent(constraint, elementName, depth + additionalDepth, addStr
					, api, countsOfConstraintElementsInOneConstraint, errorReporter);
			resString += resParentConstraint.first;
			resBool.append(resParentConstraint.second);
		} else if (constraintType == "Childrens") {
			QPair<QString, QList<QString> > resChildrensConstraint =
					GeneratorForListsOfElements::countConstraintForChildrens(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);

			resString += resChildrensConstraint.first;
			resBool.append(resChildrensConstraint.second);
		} else if (constraintType == "OutgoingLinks") {
			QPair<QString, QList<QString> > resOutgoingLinksConstraint =
					GeneratorForListsOfElements::countConstraintForOutgoingLinks(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);

			resString += resOutgoingLinksConstraint.first;
			resBool.append(resOutgoingLinksConstraint.second);
		} else if (constraintType == "IncomingLinks") {
			QPair<QString, QList<QString> > resIncomingLinksConstraint =
					GeneratorForListsOfElements::countConstraintForIncomingLinks(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);

			resString += resIncomingLinksConstraint.first;
			resBool.append(resIncomingLinksConstraint.second);
		} else if (constraintType == "OutgoingNodes") {
			QPair<QString, QList<QString> > resOutgoingNodesConstraint =
					GeneratorForListsOfElements::countConstraintForOutgoingNodes(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);

			resString += resOutgoingNodesConstraint.first;
			resBool.append(resOutgoingNodesConstraint.second);
		} else if (constraintType == "IncomingNodes") {
			QPair<QString, QList<QString> > resIncomingNodesConstraint =
					GeneratorForListsOfElements::countConstraintForIncomingNodes(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter
					, countsOfConstraintElementsInOneConstraint);

			resString += resIncomingNodesConstraint.first;
			resBool.append(resIncomingNodesConstraint.second);
		} else if (constraintType == "PropertyNode") {
			QPair<QString, QList<QString> > resPropertyNodeConstraint = countConstraintForPropertyNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, errorReporter);

			resString += resPropertyNodeConstraint.first;
			resBool.append(resPropertyNodeConstraint.second);
		}
	} else if (constraintType == "MultiOrNode") {
		QPair<QString, QList<QString> > resMultiOrNodeConstraint = countConstraintForMultiOrNode(
				constraint
				, usedElements
				, node
				, elementName
				, depth + additionalDepth
				, addStr
				, api
				, errorReporter
				, countsOfConstraintElementsInOneConstraint);

		resString += resMultiOrNodeConstraint.first;
		resBool.append(resMultiOrNodeConstraint.second);
	}

	countsOfConstraintElementsInOneConstraint[constraintType]++;

	QPair<QString, QList<QString> > resNeighborsNodes = countNeighborsElementsByOr(
			constraint
			, AuxiliaryMethods::conjunctionExpression(resBool)
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

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));

	return QPair<QString, QList<QString> >(resString, allResultBool);
}

QPair<QString, QList<QString>> GeneratorForNodeElements::countConstraintForBeginNode(const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	QString resultString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

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

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resultString, allResultBool);
}

QPair<QString, QList<QString>> GeneratorForNodeElements::countConstraintForEndNode(const Id &constraint
		, const QString &elementName
		, int depth
		, const QString &additionalString
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	QString resultString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

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

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resultString, allResultBool);
}

QPair<QString, QList<QString>> GeneratorForNodeElements::countConstraintForParent(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	QString resultString = "";
	QList<QString> resultBool;
	QList<QString> allResultBool;

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

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resultBool));

	return QPair<QString, QList<QString> >(resultString, allResultBool);
}

QPair<QString, QList<QString>> GeneratorForNodeElements::countConstraintForPropertyNode(const Id &constraint
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

	QList<QString> resBool;
	resBool.push_back(res.second);

	return QPair<QString, QList<QString> >(res.first, resBool);
}

QPair<QString, QList<QString>> GeneratorForNodeElements::countConstraintForMultiOrNode(
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
	QList<QString> curBool;
	QList<QString> resBool;

	IdList neighborNodes = AuxiliaryMethods::neighborNodesWithGivenType(constraint, "MultiOrEdge", api);
	usedElements.append(neighborNodes);

	for (const Id element : neighborNodes) {
		QPair<QString, QList<QString> > resElementConstraint;
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

	resBool.append(QString("(" + AuxiliaryMethods::disjunctionExpression(curBool) + ")"));

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString>> GeneratorForNodeElements::countNeighborsElementsByOr(
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
	QList<QString> resBool;

	IdList neighborNodes = AuxiliaryMethods::neighborNodesWithGivenType(constraint, "Or", api);

	if (!neighborNodes.isEmpty()) {
		for (const Id neighbor : neighborNodes) {
			if (!usedElements.contains(neighbor)) {
				usedElements.append(neighbor);
				QPair<QString, QList<QString> > resNeighborsConstraint;

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
						.arg(AuxiliaryMethods::conjunctionExpression(resNeighborsConstraint.second)));
			}
		}
	}

	return QPair<QString, QList<QString> >(resString, resBool);
}
