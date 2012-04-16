#include "concreateGenerator.h"
#include "../../../qrutils/generatorsUtils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace constraints::generator;
using namespace qReal;
using namespace generatorsUtils;

/// Generation target file
QString const fileNamePRO = "constraintsMetamodel.pro";
QString const fileNamePluginH = "constraintsPlugin.h";
QString const fileNamePluginCPP = "constraintsPlugin.cpp";
QString const fileNameDiagramH = "constraintsDiagram.h";
QString const fileNameDiagramCPP = "constraintsDiagram.cpp";

ConcreateGenerator::ConcreateGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &metamodelName
		)
	: AbstractGenerator(templateDirPath, outputDirPath + QString("constraints" + metamodelName + "\\"), logicalModel, errorReporter)
		, mMetamodelName(metamodelName)
{
//	QString temp = outputDirPath; //asd как собирать?
//	QList<QString> tempList = temp.split("\\");
//	temp = tempList.at(tempList.size() - 2);
//	mCallingPath = outputDirPath + temp + QString(".pro");
}

ConcreateGenerator::~ConcreateGenerator()
{
}

void ConcreateGenerator::generate()
{
	loadUtilsTemplates();

//	QString callResult; //asd как собирать?
//	loadTemplateFromFile(mCallingPath, callResult);
//	QString callPathTemplate = mTemplateUtils["@@callingPath@@"];
//	callPathTemplate.replace("@@metaModelName@@", mMetamodelName);
//	callPathTemplate.replace("@@fileNamePRO@@", fileNamePRO);
//	callResult += callPathTemplate;
//	saveOutputFile(mCallingPath, callResult);

	QString resultPRO;
	QString resultPluginH;
	QString resultPluginCPP;
	loadTemplateFromFile(fileNamePRO, resultPRO);
	loadTemplateFromFile(fileNamePluginH, resultPluginH);
	loadTemplateFromFile(fileNamePluginCPP, resultPluginCPP);

	QString hFiles;
	QString cppFiles;
	QString includeFilesPluginH;
	QString privateFieldsPluginH;
	QString ifForMainCheckPluginCPP;

	int count = 1;
	foreach (Id const &diagram, mApi.elementsByType("ConstraintsDiagram")) {
		if (!mApi.isLogicalElement(diagram)) {
			continue;
		}

		QString diagramNameTemplate = mTemplateUtils["@@diagramName@@"];
		diagramNameTemplate = replaceLanguageName(diagramNameTemplate, diagram, count);
		diagramNameTemplate.truncate(diagramNameTemplate.size() - 1);

		QString hFilesTemplate = mTemplateUtils["@@constraintsDiagramHFile@@"];
		QString cppFilesTemplate = mTemplateUtils["@@constraintsDiagramCppFile@@"];
		hFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);
		cppFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);

		QString includeFilesTemplate = mTemplateUtils["@@includeConstraintsDiagramFile@@"];
		QString privateFieldsTemplate = mTemplateUtils["@@privateFieldOfConstraintsDigram@@"];
		includeFilesTemplate.replace("@@diagramName@@", diagramNameTemplate);
		privateFieldsTemplate.replace("@@diagramName@@", diagramNameTemplate);

		QString ifForMainCheckTemplate = mTemplateUtils["@@ifForMainCheckOfConstraintsDiagram@@"];
		ifForMainCheckTemplate.replace("@@diagramName@@", diagramNameTemplate);

		//////begin
		QString resultDiagramH;
		QString resultDiagramCPP;
		loadTemplateFromFile(fileNameDiagramH, resultDiagramH);
		loadTemplateFromFile(fileNameDiagramCPP, resultDiagramCPP);
//
		QString optionalChecksForElemetsH;
		QString mainChecksForElemetsH;
		QString countOptionalCheckStatusesForElemetsCPP;
		QString countMainCheckStatusesForElemetsCPP;
		QString returnCheckStatusesOfElementsInCheckCPP;
		QString addElementsInElementsNamesCPP;

		QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP;//<elementName, appendOptionalCheckStatusOfElement>

		QMap<QString, int> elementCount;
		QList<Id> elementList= mApi.elementsByType("NodeConstraint");
		elementList.append(mApi.elementsByType("EdgeConstraint"));

		foreach (Id const &element, elementList) {
			if (!mApi.isLogicalElement(element) || mApi.parent(element) != diagram) {
				continue;
			}
			QString elementName = mApi.name(element);
			if (elementName == "(Edge Constraint)" || elementName == "(Node Constraint)") {
				elementName = "ALL";//на самом деле : вернуть ошибку(или просто проинформировать), что не задано поле "имя" для нода ограничений //qwerty
			}

			if (elementCount.contains(elementName)) {
				elementCount[elementName] ++;
			} else {
				elementCount.insert(elementName, 1);
			}
			int id = elementCount.value(elementName);

			if (id == 1) {
				QString mainCheckOfElemetH = mTemplateUtils["@@mainCheckElemet@@"];
				mainCheckOfElemetH.replace("@@elementName@@", elementName);
				mainChecksForElemetsH += mainCheckOfElemetH;

				/*countMainCheckStatusOfElemetCPP = mTemplateUtils["@@countMainCheckStatusOfElemet@@"];
				appendOptionalCheckStatusInMainCheckCPP += "";//asd //qwerty
				countMainCheckStatusOfElemetCPP.replace("@@diagramName@@", diagramNameTemplate);
				countMainCheckStatusOfElemetCPP.replace("@@elementName@@", elementName);
				countMainCheckStatusesForElemetsCPP += countMainCheckStatusOfElemetCPP;*/

				QString addElementInElementNames = mTemplateUtils["@@addElementInElementsNames@@"];
				addElementInElementNames.replace("@@elementName@@", elementName);
				addElementsInElementsNamesCPP += addElementInElementNames;

				QString returnCheckStatusOfElement = mTemplateUtils["@@returnCheckStatusOfElementInCheck@@"];
				returnCheckStatusOfElement.replace("@@elementName@@", elementName);
				returnCheckStatusesOfElementsInCheckCPP += returnCheckStatusOfElement;
			}

			QString elementNameWithId = mTemplateUtils["@@elementNameWithId@@"];
			elementNameWithId.replace("@@elementName@@", elementName).replace("@@id@@", QString::number(id));
			elementNameWithId.truncate(elementNameWithId.size() - 1);

			QString optionalCheckOfElemetH = mTemplateUtils["@@optionalCheckElemet@@"];
			optionalCheckOfElemetH.replace("@@elementNameWithId@@", elementNameWithId);
			optionalChecksForElemetsH += optionalCheckOfElemetH;

			QString countOptionalCheckStatusOfElemet = mTemplateUtils["@@countOptionalCheckStatusOfElemet@@"];
			countOptionalCheckStatusOfElemet.replace("@@elementNameWithId@@", elementNameWithId);
			countOptionalCheckStatusOfElemet.replace("@@countRealCheckStatusOfElement@@", countRealConstraintOfElement(element));
			countOptionalCheckStatusOfElemet.replace("@@errorText@@", "fail");
			countOptionalCheckStatusOfElemet.replace("@@checkStatus@@", mApi.property(element, "errorType").toString());
			countOptionalCheckStatusesForElemetsCPP += countOptionalCheckStatusOfElemet;

			QString appendOptionalCheckStatusOfElement = mTemplateUtils["@@appendOptionalCheckStatusOfElement@@"];
			appendOptionalCheckStatusOfElement.replace("@@elementNameWithId@@", elementNameWithId);
			if (appendOptionalCheckStatusInMainCheckCPP.contains(elementName)) {
				appendOptionalCheckStatusInMainCheckCPP[elementName] += appendOptionalCheckStatusOfElement;
			} else {
				appendOptionalCheckStatusInMainCheckCPP.insert(elementName, appendOptionalCheckStatusOfElement);
			}

		}

		foreach (QString curElementName, appendOptionalCheckStatusInMainCheckCPP.keys()) {
			QString appendOptionalCheckStatusOfElement = appendOptionalCheckStatusInMainCheckCPP[curElementName];
			QString countMainCheckStatusOfElemetCPP = mTemplateUtils["@@countMainCheckStatusOfElemet@@"];
			countMainCheckStatusOfElemetCPP.replace("@@elementName@@", curElementName);
			countMainCheckStatusOfElemetCPP.replace("@@appendOptionalCheckStatusesOfElement@@", appendOptionalCheckStatusOfElement);

			countMainCheckStatusesForElemetsCPP += countMainCheckStatusOfElemetCPP;
		}

		resultDiagramH.replace("@@optionalChecksForElemets@@", optionalChecksForElemetsH);
		resultDiagramH.replace("@@mainChecksForElemets@@", mainChecksForElemetsH);
		resultDiagramH.replace("@@diagramName@@", diagramNameTemplate);

		resultDiagramCPP.replace("@@countOptionalCheckStatusesForElemets@@", countOptionalCheckStatusesForElemetsCPP);
		resultDiagramCPP.replace("@@countMainCheckStatusesForElemets@@", countMainCheckStatusesForElemetsCPP);
		resultDiagramCPP.replace("@@returnCheckStatusesOfElementsInCheck@@", returnCheckStatusesOfElementsInCheckCPP);
		resultDiagramCPP.replace("@@addElementsInElementsNames@@", addElementsInElementsNamesCPP);
		resultDiagramCPP.replace("@@diagramName@@", diagramNameTemplate);
		resultDiagramCPP.replace("@@languageName@@", mApi.property(diagram, "languageName").toString());
//
		saveOutputFile(QString("constraints" + diagramNameTemplate + ".h"), resultDiagramH);
		saveOutputFile(QString("constraints" + diagramNameTemplate + ".cpp"), resultDiagramCPP);
		//////

		count++;

		hFiles += hFilesTemplate;
		cppFiles += cppFilesTemplate;

		includeFilesPluginH += includeFilesTemplate;
		privateFieldsPluginH += privateFieldsTemplate;

		ifForMainCheckPluginCPP += ifForMainCheckTemplate;
	}

	resultPRO.replace("@@constraintsDiagramHFiles@@", hFiles);
	resultPRO.replace("@@constraintsDiagramCppFiles@@", cppFiles);

	resultPluginH.replace("@@includeConstraintsDiagramFiles@@", includeFilesPluginH);
	resultPluginH.replace("@@privateFieldsOfConstraintsDigrams@@", privateFieldsPluginH);

	resultPluginCPP.replace("@@ifForMainCheckOfConstraintsDiagrams@@", ifForMainCheckPluginCPP);
	resultPluginCPP.replace("@@metaModelName@@", mMetamodelName);

	saveOutputFile(fileNamePRO, resultPRO);
	saveOutputFile(fileNamePluginH, resultPluginH);
	saveOutputFile(fileNamePluginCPP, resultPluginCPP);
}

QString ConcreateGenerator::replaceLanguageName(QString string, Id const &diagram, int count)
{
	string.replace("@@languageName@@", mApi.property(diagram, "languageName").toString())
			.replace("@@id@@", QString::number(count));
	return string;
}

QString ConcreateGenerator::countRealConstraintOfElement(Id const &element)
{
	QString resString;
	if (element.element() == "EdgeConstraint") {
		resString = countRealConstraintForEdgeElement(element, "element", "edgeRes", 1, "	");
		resString += "	res = edgeRes_1;\n";
	} else if (element.element() == "NodeConstraint") {
		resString = countRealConstraintForNodeElement(element, "element", "nodeRes", 1, "	");
		resString += "	res = nodeRes_1;\n";
	}
	return resString;
}

QString ConcreateGenerator::countRealConstraintForEdgeElement(Id const &element, QString elementName, QString resultName, int depth, QString addStr)
{
	IdList list = mApi.children(element);
	QString resString = "";
	QList<QString> resBool;

	foreach (Id const &constraint, list) {
		QString constraintType = constraint.element();
		QString exists;
		if (constraintType == "BeginNode") {
			resString += addStr + "qReal::Id newBeginNodeName_" + QString::number(depth) + " = logicalApi.from(" + elementName + ");\n";
			resString += countRealConstraintForNodeElement(constraint, "newBeginNodeName_" + QString::number(depth) , "beginNodeRes", depth + 1, addStr);
			resBool.push_back("beginNodeRes_" + QString::number(depth + 1));

			resString += addStr + "bool beginNodeRes_" + QString::number(depth) + " = ";
			exists = mApi.property(constraint, "exists").toBool() ? "!=" : "==";
			resString += "(newBeginNodeName_" + QString::number(depth) + " " + exists + "qReal::Id::rootId());\n";// && beginNodeRes_" + QString::number(depth + 1) + ";\n";
			resBool.push_back("beginNodeRes_" + QString::number(depth));

		} else if (constraintType == "EndNode") {
			resString += addStr + "qReal::Id newEndNodeName_" + QString::number(depth) + " = logicalApi.to(" + elementName + ");\n";
			resString += countRealConstraintForNodeElement(constraint, "newEndNodeName_" + QString::number(depth), "endNodeRes", depth + 1, addStr);
			resBool.push_back("endNodeRes_" + QString::number(depth + 1));

			resString += addStr + "bool endNodeRes_" + QString::number(depth) + " = ";
			exists = mApi.property(constraint, "exists").toBool() ? "!=" : "==";
			resString += "(newEndNodeName_" + QString::number(depth) + " " + exists + "qReal::Id::rootId());\n";// && endNodeRes_" + QString::number(depth + 1) + ";\n";
			resBool.push_back("endNodeRes_" + QString::number(depth));

		}
	}
	if (resBool.empty()) {
		resBool.push_back("true"); //"true";//вернуть ошибку; т.е мы не смогли сгенерить ограничения для элемента
	}
	resString += addStr + ("	bool " + resultName + "_" + QString::number(depth) + " = " + resBool.first());
	resBool.pop_front();
	foreach (QString curBool, resBool) {
		resString += " && " + curBool;
	}
	resString += ";\n";

	return resString;
}

QString ConcreateGenerator::countRealConstraintForNodeElement(Id const &element, QString elementName, QString resultName, int depth, QString addStr)
{
	IdList list = mApi.children(element);
	QString resString = "";//resultName + "_" + QString::number(depth) + " = ";
	QList<QString> resBool;

	foreach (Id const &constraint, list) {
		QString constraintType = constraint.element();
		if (constraintType == "Parent") {
			resString += addStr + "qReal::Id newParentName_" + QString::number(depth) + " = logicalApi.parent(" + elementName + ");\n";
			resString += addStr + "bool mainParentRes_" + QString::number(depth) + " = true;\n";
			resString += addStr + "if (newParentName_" + QString::number(depth) + " != qReal::Id::rootId()) {\n";
			resString += addStr + countRealConstraintForNodeElement(constraint, "newParentName_" + QString::number(depth) , "parentNodeRes", depth + 1, addStr + "	");
			resString += addStr + "	mainParentRes_" + QString::number(depth) + " = parentNodeRes_" + QString::number(depth + 1) + ";\n	}\n";
			resBool.push_back("mainParentRes_" + QString::number(depth));
		} else if (constraintType == "Childrens") {
			resString += addStr + "qReal::IdList newChildrenNamesList_" + QString::number(depth) + " = logicalApi.children(" + elementName + ");\n";
			resString += addStr + "bool mainChildrenRes_" + QString::number(depth + 1) + " = true;\n";
			resString += addStr + "foreach (qReal::Id const &node, newChildrenNamesList_" + QString::number(depth) + ") {\n";
			resString += countRealConstraintForNodeElement(constraint, "node", "childrenRes", depth + 1, addStr + "	");
			resString += addStr + "	mainChildrenRes_" + QString::number(depth + 1) + " = mainChildrenRes_" + QString::number(depth + 1) + " && childrenRes_" + QString::number(depth + 1) + ";\n	}\n";
			resBool.push_back("mainChildrenRes_" + QString::number(depth + 1));
		} else if (constraintType == "outgoingLinks") {
			resString += addStr + "qReal::IdList newOutgoingLinksNamesList_" + QString::number(depth) + " = logicalApi.outgoingLinks(" + elementName + ");\n";
			resString += addStr + "bool mainOutgoingLinksRes_" + QString::number(depth + 1) + " = true;\n";
			resString += addStr + "foreach (qReal::Id const &edge, newOutgoingLinksNamesList_" + QString::number(depth) + ") {\n";
			resString += countRealConstraintForNodeElement(constraint, "edge", "outgoingLinksRes", depth + 1, addStr + "	");
			resString += addStr + "	mainOutgoingLinksRes_" + QString::number(depth + 1) + " = mainOutgoingLinksRes_" + QString::number(depth + 1) + " && outgoingLinksRes_" + QString::number(depth + 1) + ";\n	}\n";
			resBool.push_back("mainOutgoingLinksRes_" + QString::number(depth + 1));
		} else if (constraintType == "incomingLinks") {
			resString += addStr + "qReal::IdList newIncomingLinksNamesList_" + QString::number(depth) + " = logicalApi.incomingLinks(" + elementName + ");\n";
			resString += addStr + "bool mainIncomingLinksRes_" + QString::number(depth + 1) + " = true;\n";
			resString += addStr + "foreach (qReal::Id const &edge, newIncomingLinksNamesList_" + QString::number(depth) + ") {\n";
			resString += countRealConstraintForNodeElement(constraint, "edge", "incomingLinksRes", depth + 1, addStr + "	");
			resString += addStr + "	mainIncomingLinksRes_" + QString::number(depth + 1) + " = mainIncominggLinksRes_" + QString::number(depth + 1) + " && incomingLinksRes_" + QString::number(depth + 1) + ";\n	}\n";
			resBool.push_back("mainIncomingLinksRes_" + QString::number(depth + 1));
		} else if (constraintType == "outgoingNodes") {
			resString += addStr + "qReal::IdList newOutgoingNodesNamesList_" + QString::number(depth) + " = logicalApi.outgoingNodes(" + elementName + ");\n";
			resString += addStr + "bool mainOutgoingNodesRes_" + QString::number(depth + 1) + " = true;\n";
			resString += addStr + "foreach (qReal::Id const &node, newOutgoingNodesNamesList_" + QString::number(depth) + ") {\n";
			resString += countRealConstraintForNodeElement(constraint, "node", "outgoingNodesRes", depth + 1, addStr + "	");
			resString += addStr + "	mainOutgoingNodesRes_" + QString::number(depth + 1) + " = mainOutgoingNodesRes_" + QString::number(depth + 1) + " && outgoingNodesRes_" + QString::number(depth + 1) + ";\n	}\n";
			resBool.push_back("mainOutgoingNodesRes_" + QString::number(depth + 1));
		} else if (constraintType == "incomingNodes") {
			resString += addStr + "qReal::IdList newIncomingNodesNamesList_" + QString::number(depth) + " = logicalApi.incomingNodes(" + elementName + ");\n";
			resString += addStr + "bool mainIncomingNodesRes_" + QString::number(depth + 1) + " = true;\n";
			resString += addStr + "foreach (qReal::Id const &node, newIncomingNodesNamesList_" + QString::number(depth) + ") {\n";
			resString += countRealConstraintForNodeElement(constraint, "node", "incomingNodesRes", depth + 1, addStr + "	");
			resString += addStr + "	mainIncomingNodesRes_" + QString::number(depth + 1) + " = mainIncominggNodesRes_" + QString::number(depth + 1) + " && incomingNodesRes_" + QString::number(depth + 1) + ";\n	}\n";
			resBool.push_back("mainIncomingNodesRes_" + QString::number(depth + 1));
		} else if (constraintType == "PropertyNode") {
			QString property = mApi.property(constraint, "property").toString();
			QString sign = mApi.property(constraint, "sign").toString();
			if (sign == "=") {
				sign += "=";
			}
			QString value = mApi.property(constraint, "value").toString();
			resString += addStr + "bool propertyNodeRes_" + QString::number(depth) + " = true;\n";
			resString += addStr + "if (logicalApi.hasProperty(" + elementName + ", \"" + property + "\")) {\n";
			resString += addStr + "	propertyNodeRes_" + QString::number(depth) + " = (logicalApi.property(" + elementName + ", \"" + property + "\").toString() " + sign + " \"" + value + "\");\n";
			resString += addStr + "}\n";
			resBool.push_back("propertyNodeRes_" + QString::number(depth));
		}
	}

	if (resBool.empty()) {//asd_copypast
		resBool.push_back("true");//вернуть ошибку; т.е мы не смогли сгенерить ограничения для элемента
	}
	resString += addStr + ("bool " + resultName + "_" + QString::number(depth) + " = " + resBool.first());
	resBool.pop_front();
	foreach (QString curBool, resBool) {
		resString += " && " + curBool;
	}
	resString += ";\n";

	return resString;
}
