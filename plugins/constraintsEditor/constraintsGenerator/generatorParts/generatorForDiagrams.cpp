#include "generatorForDiagrams.h"
#include "generatorForProperties.h"
#include "generatorForEdgeElements.h"
#include "generatorForNodeElements.h"

using namespace constraints::generator::generatorParts;

QString GeneratorForDiagrams::countRealConstraintOfDiagramElement(
		const qReal::Id &constraintElement
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, const qrRepo::LogicalRepoApi &api
		, qReal::ErrorReporterInterface &errorReporter)
{
	QString resultString = "";

	for (const QString &nameOfConstrainElement : countsOfConstraintElementsInOneConstraint.keys()) {
		countsOfConstraintElementsInOneConstraint[nameOfConstrainElement] = 0;
	}

	QPair<bool, QString> selectionResult = handleConstraintsSelection(constraintElement, api, errorReporter);
	if (selectionResult.first) {
		resultString += selectionResult.second;
	}

	if (constraintElement.element() == "EdgeConstraint") {
		resultString += GeneratorForEdgeElements::countRealConstraintForEdgeElement(
					constraintElement
					, "element"
					, "edgeRes"
					, 1
					, "	"
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter
					);
		resultString += "	res = edgeRes_1;\n";

	} else if (constraintElement.element() == "NodeConstraint") {
		resultString += GeneratorForNodeElements::countRealConstraintForNodeElement(
					constraintElement
					, "element"
					, "nodeRes"
					, 1
					, "	"
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter
					);
		resultString += "	res = nodeRes_1;\n";

	} else if (constraintElement.element() == "EdgesConstraint") {
		resultString += GeneratorForEdgeElements::countRealConstraintForEdgeElement(
					constraintElement
					, "element"
					, "allEdgesRes"
					, 1
					, "		"
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter
					);
		resultString += "	res = allEdgesRes_1;\n";

	} else if (constraintElement.element() == "NodesConstraint") {
		resultString += GeneratorForNodeElements::countRealConstraintForNodeElement(
					constraintElement
					, "element"
					, "allNodesRes"
					, 1
					, "		"
					, api
					, countsOfConstraintElementsInOneConstraint
					, errorReporter);
		resultString += "	res = allNodesRes_1;\n";
	}

	if (selectionResult.first) {
		resultString += "	}\n";
	}

	return resultString;
}

QPair<bool, QString> GeneratorForDiagrams::handleConstraintsSelection(
		const qReal::Id &constraintElement
		, const qrRepo::LogicalRepoApi &api
		, qReal::ErrorReporterInterface &errorReporter)
{
	QString constraintElementType = constraintElement.element();

	if (constraintElementType == "EdgesConstraint" || constraintElementType == "NodesConstraint") {
		QString resultString = "";
		QList<QString> resBool;

		bool neededSelectionByProperty  = false;

		if (api.hasProperty(constraintElement, "selection")) {
			const QString selection = api.property(constraintElement, "selection").toString();
			neededSelectionByProperty  = (!selection.isEmpty()) && (selection.compare("all", Qt::CaseInsensitive) != 0);

			if (neededSelectionByProperty) {
				QList<QString> selectionList = selection.split(" ");
				QPair<QString, QString > selectionResByProperty =
						GeneratorForProperties::countPropertyCharacteristicForConstraintElement(
						constraintElement
						, "elementsSelectionByProperty"
						, "false"
						, selectionList.at(0)
						, selectionList.at(1)
						, selectionList.at(2)
						, "element"
						, 1
						, "	"
						, errorReporter);

				resultString += selectionResByProperty.first;
				resBool.push_back(selectionResByProperty.second);
			}
		}

		if (resBool.size() == 1) {
			resultString += QString("	if (%1) {\n").arg(resBool.at(0));
		} else {
			resultString += "	if (true) {\n";
		}

		return QPair<bool, QString>(neededSelectionByProperty, resultString);
	} else {
		return QPair<bool, QString>(false, "");
	}
}
