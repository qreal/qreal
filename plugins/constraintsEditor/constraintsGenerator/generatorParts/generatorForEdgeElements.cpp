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
		, const QString &addStr
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	return GeneratorForElements::countRealConstraintForElement(
			constraintElement, edge, elementName, resultName, depth, addStr, api
			, countsOfConstraintElementsInOneConstraint, errorReporter);
}

QPair<QString, QList<QString>> GeneratorForEdgeElements::countRealConstraintForOneEdgeElement(
		const Id &constraint
		, IdList &usedElements
		, const QString &elementName
		, const int depth
		, const QString &addStr
		, const qrRepo::LogicalRepoApi &api
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

	QPair<QString, QList<QString> > nodeConstraint;
	if ((!GeneratorForLinks::linkWithGivenTypeExists(constraint, "MultiOrEdge", api)) || isMultiOr) {
		if (constraintType == "BeginNode") {
			nodeConstraint = GeneratorForNodeElements::countConstraintForBeginNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter);
		} else if (constraintType == "EndNode") {
			nodeConstraint = GeneratorForNodeElements::countConstraintForEndNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter);
		} else if (constraintType == "PropertyNode") {
			nodeConstraint = GeneratorForNodeElements::countConstraintForPropertyNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
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
				, addStr
				, api
				, errorReporter
				, countsOfConstraintElementsInOneConstraint);
	}

	resString += nodeConstraint.first;
	resBool.append(nodeConstraint.second);

	countsOfConstraintElementsInOneConstraint[constraintType]++;

	QPair<QString, QList<QString> > resNeighborsNodes = GeneratorForNodeElements::countNeighborsElementsByOr(
			constraint
			, GeneratorForExpressions::conjunctionExpression(resBool)
			, usedElements
			, edge
			, elementName
			, depth + additionalDepth
			, addStr
			, api
			, countsOfConstraintElementsInOneConstraint
			, errorReporter);

	resString += resNeighborsNodes.first;

	if (!resNeighborsNodes.second.isEmpty()) {
		resBool.append(resNeighborsNodes.second);
	}

	allResultBool.append(GeneratorForExpressions::conjunctionExpression(resBool));

	return QPair<QString, QList<QString> >(resString, allResultBool);
}
