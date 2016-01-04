#include "countRealConstraintsMethods.h"

using namespace constraints::generator::generatorUtils;
using namespace qReal;

QString CountRealConstraintsMethods::countRealConstraintOfDiagramElement(const Id &constraintElement)
{
//	QString resString = "";

//	for (const QString &nameOfConstraintElement : mCountsOfConstraintElementsInOneConstraint.keys()) {
//		// иницилизируем счетчики для элементов внутри одного основного блока ограничений
//		mCountsOfConstraintElementsInOneConstraint[nameOfConstraintElement] = 0;
//	}

//	QPair<bool, QString> selectionResult = handleConstraintsSelection(constraintElement);
//	if (selectionResult.first) {
//		resString += selectionResult.second;
//	}

//	if (constraintElement.element() == "EdgeConstraint") {
//		resString += countRealConstraintForEdgeElement(constraintElement, "element", "edgeRes", 1, "	");
//		resString += "	res = edgeRes_1;\n";

//	} else if (constraintElement.element() == "NodeConstraint") {
//		resString += countRealConstraintForNodeElement(constraintElement, "element", "nodeRes", 1, "	");
//		resString += "	res = nodeRes_1;\n";

//	} else if (constraintElement.element() == "EdgesConstraint") {
//		resString += countRealConstraintForEdgeElement(constraintElement, "element", "allEdgesRes", 1, "		");
//		resString += "	res = allEdgesRes_1;\n";

//	} else if (constraintElement.element() == "NodesConstraint") {
//		resString += countRealConstraintForNodeElement(constraintElement, "element", "allNodesRes", 1, "		");
//		resString += "	res = allNodesRes_1;\n";
//	}

//	if (selectionResult.first) {
//		resString += "	}\n";
//	}

//	return resString;
}
