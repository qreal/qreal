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

QString const keywordForAllEdges = "AllEdges";
QString const keywordForAllNodes = "AllNodes";
QString const keywordForAllLanguages = "AllLanguages";

ConcreateGenerator::ConcreateGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, QString const &pathToQReal
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &metamodelLanguageName
		, QString const &constraintsMetamodelName
		)
	: AbstractGenerator(templateDirPath, outputDirPath + QString("\\constraints" + metamodelLanguageName + "\\"), logicalModel, errorReporter)
	, mPathToQReal(pathToQReal), mMetamodelName(metamodelLanguageName), mConstraintsName(constraintsMetamodelName)
{
	mPathToQReal.replace("\\", "/");
}

ConcreateGenerator::~ConcreateGenerator()
{
}

QString ConcreateGenerator::constraintModelFullName()
{
	return mOutputDirPath;// + "constraints" + mMetamodelName + ".pro";
}

QString ConcreateGenerator::constraintModelName() //i.e. pliginName
{
	return "constraints" + mMetamodelName;
}

QString ConcreateGenerator::constraintModelId() //i.e. pliginId
{
	return mConstraintsName;
}

ConcreateGenerator::NeededStringsForConcreateGenerate ConcreateGenerator::generateCommonNeededPartsForElements(QString elementName)
{
	QString mainChecksForElemetsH;
	QString returnCheckStatusesOfElementsInCheckCPP;
	QString addElementsInElementsNamesCPP;

	QString mainCheckOfElemetH = mTemplateUtils["@@mainCheckElemet@@"];
	mainCheckOfElemetH.replace("@@elementName@@", elementName);
	mainChecksForElemetsH += mainCheckOfElemetH;

	QString addElementInElementNames = mTemplateUtils["@@addElementInElementsNames@@"];
	addElementInElementNames.replace("@@elementName@@", elementName);
	addElementsInElementsNamesCPP += addElementInElementNames;

	if (elementName == keywordForAllNodes || elementName == keywordForAllEdges) {
		QString metaTypeName = "";
		mUsedMetaTypeInCheck = true;
		if (elementName == keywordForAllNodes) {
			metaTypeName = "node";
		} else if (elementName == keywordForAllEdges) {
			metaTypeName = "edge";
		}
		QString returnCheckStatusOfElementByMetaType = mTemplateUtils["@@returnCheckStatusOfElementByMetaTypeInCheck@@"];
		returnCheckStatusOfElementByMetaType.replace("@@elementName@@", elementName);
		returnCheckStatusOfElementByMetaType.replace("@@metaType@@", metaTypeName);
		returnCheckStatusesOfElementsInCheckCPP += returnCheckStatusOfElementByMetaType;
	} else {
		QString returnCheckStatusOfElement = mTemplateUtils["@@returnCheckStatusOfElementInCheck@@"];
		returnCheckStatusOfElement.replace("@@elementName@@", elementName);
		returnCheckStatusesOfElementsInCheckCPP += returnCheckStatusOfElement;
	}

	return NeededStringsForConcreateGenerate("", mainChecksForElemetsH, "", "", "", returnCheckStatusesOfElementsInCheckCPP, addElementsInElementsNamesCPP);
}

QString ConcreateGenerator::generateMainCheckStatusesForElemetsCPP(QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP)
{
	QString countMainCheckStatusesForElemetsCPP;
	foreach (QString curElementName, appendOptionalCheckStatusInMainCheckCPP.keys()) {
		QString appendOptionalCheckStatusOfElement = appendOptionalCheckStatusInMainCheckCPP[curElementName];
		QString countMainCheckStatusOfElemetCPP = mTemplateUtils["@@countMainCheckStatusOfElemet@@"];
		countMainCheckStatusOfElemetCPP.replace("@@elementName@@", curElementName);
		countMainCheckStatusOfElemetCPP.replace("@@appendOptionalCheckStatusesOfElement@@", appendOptionalCheckStatusOfElement);

		countMainCheckStatusesForElemetsCPP += countMainCheckStatusOfElemetCPP;
	}
	return countMainCheckStatusesForElemetsCPP;
}

ConcreateGenerator::NeededStringsForConcreateGenerate ConcreateGenerator::generateNeededPartsForDiagramFiles(Id const &diagram)
{
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
	elementList.append(mApi.elementsByType("NodesConstraint"));
	elementList.append(mApi.elementsByType("EdgesConstraint"));

	foreach (Id const &element, elementList) {
		if (!mApi.isLogicalElement(element) || mApi.parent(element) != diagram) {
			continue;
		}
		QString elementName = mApi.name(element);
		if (elementName == "(Edge Constraint)" || elementName == "(Node Constraint)") {
			mErrorReporter.addCritical("Name of constraintNode not found!", element);
		}
		if (elementName == "(Edges Constraint)"
				|| (element.element() == "EdgesConstraint" && ((elementName.compare("all", Qt::CaseInsensitive)) == 0)
										&& ((elementName.compare("alledges", Qt::CaseInsensitive)) == 0)) ) {
			elementName = keywordForAllEdges; //по умолчанию; ошибки не надо
		}
		if (elementName == "(Nodes Constraint)"
				|| (element.element() == "NodesConstraint" && ((elementName.compare("all", Qt::CaseInsensitive)) == 0)
										&& ((elementName.compare("allnodes", Qt::CaseInsensitive)) == 0)) ) {
			elementName = keywordForAllNodes; //по умолчанию; ошибки не надо
		}

		if (elementCount.contains(elementName)) {
			elementCount[elementName] ++;
		} else {
			elementCount.insert(elementName, 1);
		}

		int id = elementCount.value(elementName);
		if (id == 1) {
			NeededStringsForConcreateGenerate neededString = generateCommonNeededPartsForElements(elementName);
			mainChecksForElemetsH += neededString.mainChecksForElemetsH;
			returnCheckStatusesOfElementsInCheckCPP += neededString.returnCheckStatusesOfElementsInCheckCPP;
			addElementsInElementsNamesCPP += neededString.addElementsInElementsNamesCPP;
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
		countOptionalCheckStatusOfElemet.replace("@@errorText@@", mApi.property(element, "errorText").toString());
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

	QString prefixForReturnCheckStatusesOfElementsInCheckCPP = "";
	if (mUsedMetaTypeInCheck) {
		prefixForReturnCheckStatusesOfElementsInCheckCPP = mTemplateUtils["@@prefixForReturnCheckStatusOfElementByMetaTypeInCheck@@"];
	}

	countMainCheckStatusesForElemetsCPP += generateMainCheckStatusesForElemetsCPP(appendOptionalCheckStatusInMainCheckCPP);

	return NeededStringsForConcreateGenerate(optionalChecksForElemetsH, mainChecksForElemetsH, countOptionalCheckStatusesForElemetsCPP, countMainCheckStatusesForElemetsCPP
											, prefixForReturnCheckStatusesOfElementsInCheckCPP, returnCheckStatusesOfElementsInCheckCPP, addElementsInElementsNamesCPP);
}

void ConcreateGenerator::generateDiagramFiles(Id const &diagram, QString diagramNameTemplate)
{
	QString resultDiagramH;
	QString resultDiagramCPP;
	loadTemplateFromFile(fileNameDiagramH, resultDiagramH);
	loadTemplateFromFile(fileNameDiagramCPP, resultDiagramCPP);

	NeededStringsForConcreateGenerate needeStrings = generateNeededPartsForDiagramFiles(diagram);

	resultDiagramH.replace("@@optionalChecksForElemets@@", needeStrings.optionalChecksForElemetsH);
	resultDiagramH.replace("@@mainChecksForElemets@@", needeStrings.mainChecksForElemetsH);
	resultDiagramH.replace("@@diagramName@@", diagramNameTemplate);
	resultDiagramH.replace("@@pathToQReal@@", mPathToQReal);

	resultDiagramCPP.replace("@@countOptionalCheckStatusesForElemets@@", needeStrings.countOptionalCheckStatusesForElemetsCPP);
	resultDiagramCPP.replace("@@countMainCheckStatusesForElemets@@", needeStrings.countMainCheckStatusesForElemetsCPP);
	resultDiagramCPP.replace("@@prefixForReturnCheckStatusesOfElementsInCheck@@", needeStrings.prefixForReturnCheckStatusesOfElementsInCheckCPP);
	resultDiagramCPP.replace("@@returnCheckStatusesOfElementsInCheck@@", needeStrings.returnCheckStatusesOfElementsInCheckCPP);
	resultDiagramCPP.replace("@@addElementsInElementsNames@@", needeStrings.addElementsInElementsNamesCPP);
	resultDiagramCPP.replace("@@diagramName@@", diagramNameTemplate);
	resultDiagramCPP.replace("@@languageName@@", correctedLanguageName(diagram));

	saveOutputFile(QString("constraints" + diagramNameTemplate + ".h"), resultDiagramH);
	saveOutputFile(QString("constraints" + diagramNameTemplate + ".cpp"), resultDiagramCPP);
}

ConcreateGenerator::NeededStringsForCommonGenerate ConcreateGenerator::generateNeededPartsForAllConstraintsDiagram()
{
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
		mUsedMetaTypeInCheck = false;
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
		ifForMainCheckTemplate.replace("@@keywordForAllEdges@@", keywordForAllEdges);
		ifForMainCheckTemplate.replace("@@keywordForAllNodes@@", keywordForAllNodes);
		ifForMainCheckTemplate.replace("@@keywordForAllLanguages@@", keywordForAllLanguages);

		generateDiagramFiles(diagram, diagramNameTemplate);
		count++;

		hFiles += hFilesTemplate;
		cppFiles += cppFilesTemplate;

		includeFilesPluginH += includeFilesTemplate;
		privateFieldsPluginH += privateFieldsTemplate;

		ifForMainCheckPluginCPP += ifForMainCheckTemplate;
	}
	return NeededStringsForCommonGenerate(hFiles, cppFiles, includeFilesPluginH, privateFieldsPluginH, ifForMainCheckPluginCPP);
}

void ConcreateGenerator::generate()
{
	loadUtilsTemplates();

	QString resultPRO;
	QString resultPluginH;
	QString resultPluginCPP;
	loadTemplateFromFile(fileNamePRO, resultPRO);
	loadTemplateFromFile(fileNamePluginH, resultPluginH);
	loadTemplateFromFile(fileNamePluginCPP, resultPluginCPP);

	NeededStringsForCommonGenerate neededStrings = generateNeededPartsForAllConstraintsDiagram();

	resultPRO.replace("@@pathToQReal@@", mPathToQReal);
	resultPRO.replace("@@metamodelName@@", mMetamodelName);
	resultPRO.replace("@@constraintsDiagramHFiles@@", neededStrings.hFiles);
	resultPRO.replace("@@constraintsDiagramCppFiles@@", neededStrings.cppFiles);

	resultPluginH.replace("@@includeConstraintsDiagramFiles@@", neededStrings.includeFilesPluginH);
	resultPluginH.replace("@@privateFieldsOfConstraintsDigrams@@", neededStrings.privateFieldsPluginH);
	resultPluginH.replace("@@pathToQReal@@", mPathToQReal);

	resultPluginCPP.replace("@@metamodelName@@", mMetamodelName);
	resultPluginCPP.replace("@@ifForMainCheckOfConstraintsDiagrams@@", neededStrings.ifForMainCheckPluginCPP);
	resultPluginCPP.replace("@@constraintsPluginId@@", mConstraintsName);

	saveOutputFile(QString("constraints" + mMetamodelName + ".pro"), resultPRO);
	saveOutputFile(QString("constraints" + mMetamodelName + "Plugin.h"), resultPluginH);
	saveOutputFile(QString("constraints" + mMetamodelName + "Plugin.cpp"), resultPluginCPP);
}

QString ConcreateGenerator::correctedLanguageName(Id const &diagram)
{
	QString languageName = mApi.property(diagram, "languageName").toString();
	if (languageName.isEmpty()) {
		mErrorReporter.addCritical("LanguageName of conatraints diagram not found.", diagram);
		return;
	}
	if ((languageName.compare("all", Qt::CaseInsensitive) == 0) || (languageName.compare(keywordForAllLanguages, Qt::CaseInsensitive) == 0)) {
		languageName = keywordForAllLanguages;
	}
	return languageName;
}

QString ConcreateGenerator::replaceLanguageName(QString string, Id const &diagram, int count)
{
	string.replace("@@languageName@@", correctedLanguageName(diagram))
			.replace("@@id@@", QString::number(count));
	return string;
}

QPair<bool, QString> ConcreateGenerator::handleConstraintsSelection(Id const &constraintElement)
{
	QString constraintElementType = constraintElement.element();
	if (constraintElementType == "EdgesConstraint" || constraintElementType == "NodesConstraint") { //т.к. выборка имеет смысл только для этих двух эелементов
		QString resString = "";
		QList<QString> resBool;

		bool neededSelectionByProperty  = false;
		if (mApi.hasProperty(constraintElement, "selection")) {
			QString selection = mApi.property(constraintElement, "selection").toString();
			neededSelectionByProperty  = (!selection.isEmpty()) && (selection.compare("all", Qt::CaseInsensitive) != 0);
			if (neededSelectionByProperty) {
				QList<QString> selectionList = selection.split(" ");
				QPair<QString, QString > selectionResByProperty = countPropertyCharacteristicForConstraintElement(constraintElement, "elementsSelectionByProperty", "false"
																												  , selectionList.at(0), selectionList.at(1), selectionList.at(2), "element", 1, "	");
				resString += selectionResByProperty.first;
				resBool.push_back(selectionResByProperty.second);
			}
		}

		if (resBool.size() == 1) {
			resString += "	if (" + resBool.at(0) + ") {\n";
		} else { //qwerty_не имеет смысла (т.к. тут neededSelection = false) _это не будет использовано по смыслу всего этого действа
			resString += "	if (true) {\n";
		}
		return QPair<bool, QString>(neededSelectionByProperty, resString);
	} else {
		return QPair<bool, QString>(false, "");
	}
}

QString ConcreateGenerator::countRealConstraintOfElement(Id const &constraintElement)
{
	QString resString = "";

	foreach (QString nameOfConstrainElement, mCountsOfConstraintElementsInOneConstraint.keys()) { // иницилизируем счетчики для элементов внутри одного основного блока ограничений
		mCountsOfConstraintElementsInOneConstraint[nameOfConstrainElement] = 0;
	}

	QPair<bool, QString> selectionResult = handleConstraintsSelection(constraintElement);
	if (selectionResult.first) {
		resString += selectionResult.second;
	}

	if (constraintElement.element() == "EdgeConstraint") {
		resString += countRealConstraintForEdgeElement(constraintElement, "element", "edgeRes", 1, "	");
		resString += "	res = edgeRes_1;\n";

	} else if (constraintElement.element() == "NodeConstraint") {
		resString += countRealConstraintForNodeElement(constraintElement, "element", "nodeRes", 1, "	");
		resString += "	res = nodeRes_1;\n";

	} else if (constraintElement.element() == "EdgesConstraint") {
		resString += countRealConstraintForEdgeElement(constraintElement, "element", "allEdgesRes", 1, "		");
		resString += "	res = allEdgesRes_1;\n";

	} else if (constraintElement.element() == "NodesConstraint") {
		resString += countRealConstraintForNodeElement(constraintElement, "element", "allNodesRes", 1, "		");
		resString += "	res = allNodesRes_1;\n";
	}

	if (selectionResult.first) {
		resString += "	}\n";
	}

	return resString;
}

QString ConcreateGenerator::generateExistsProperty(QString const &resElementName, QString const &elementName, Id const &constraint, int depth, QString addStr)
{
	QString resString = "";
	QString exists;

	exists = mApi.property(constraint, "exists").toString();
	resString += addStr + "bool " + resElementName + "_" + QString::number(depth) + " = ";

	if (exists == "true" || exists == "false") {
		if (exists == "true") {
			exists = "!=";
		} else if (exists == "false") {
			exists = "==";
		}
		resString += "(" + elementName + "_" + QString::number(depth) + " " + exists + " qReal::Id::rootId());\n";
	} else {
		resString += "true;\n";
	}
	return resString;
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForBeginNode(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	resString += addStr + "qReal::Id newBeginNodeName_" + QString::number(depth) + " = logicalApi.from(" + elementName + ");\n";
	resString += countRealConstraintForNodeElement(constraint, "newBeginNodeName_" + QString::number(depth) , "beginNodeRes", depth + 1, addStr);
	resBool.push_back("beginNodeRes_" + QString::number(depth + 1));

	resString += generateExistsProperty("beginNodeRes", "newBeginNodeName", constraint, depth, addStr);
	resBool.push_back("beginNodeRes_" + QString::number(depth));

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForEndNode(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	resString += addStr + "qReal::Id newEndNodeName_" + QString::number(depth) + " = logicalApi.to(" + elementName + ");\n";
	resString += countRealConstraintForNodeElement(constraint, "newEndNodeName_" + QString::number(depth), "endNodeRes", depth + 1, addStr);
	resBool.push_back("endNodeRes_" + QString::number(depth + 1));

	resString += generateExistsProperty("endNodeRes", "newEndNodeName", constraint, depth, addStr);
	resBool.push_back("endNodeRes_" + QString::number(depth));

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForParent(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	resString += addStr + "qReal::Id newParentName_" + QString::number(depth) + " = logicalApi.parent(" + elementName + ");\n";
	resString += addStr + "bool mainParentRes_" + QString::number(depth) + " = true;\n";
	resString += addStr + "if (newParentName_" + QString::number(depth) + " != qReal::Id::rootId()) {\n";
	resString += addStr + countRealConstraintForNodeElement(constraint, "newParentName_" + QString::number(depth) , "parentNodeRes", depth + 1, addStr + "	");
	resString += addStr + "	mainParentRes_" + QString::number(depth) + " = parentNodeRes_" + QString::number(depth + 1) + ";\n";
	resString += addStr + "	}\n";
	resBool.push_back("mainParentRes_" + QString::number(depth));

	resString += generateExistsProperty("parentRes", "newParentName", constraint, depth, addStr);
	resBool.push_back("parentRes_" + QString::number(depth));

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForListOfElements(Id const &constraint, QString elementName, QString resElementName, QString functionName, QString resType, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;
	QString count = mApi.property(constraint, "count").toString(); //qwerty_think over_count
	bool neededCount  = (!count.isEmpty());

	QString selection = mApi.property(constraint, "selection").toString();
	bool neededSelection  = (!selection.isEmpty()) && (selection.compare("all", Qt::CaseInsensitive) != 0);

	if (neededCount) {
		resString += addStr + "int tempCount" + resElementName + "_" + QString::number(depth) + " = 0;\n";
	}

	resString += addStr + "qReal::IdList new" + resElementName + "NamesList_" + QString::number(depth) + " = logicalApi." + functionName + "(" + elementName + ");\n";

	resString += addStr + "bool main" + resElementName + "Res_" + QString::number(depth + 1) + " = true;\n";
	QString curElementOfList = resType + resElementName + "_" +QString::number(depth);
	resString += addStr + "foreach (qReal::Id const &" + curElementOfList + ", new" + resElementName + "NamesList_" + QString::number(depth) + ") {\n";
	if (neededSelection) {
		QList<QString> selectionList = selection.split(" ");
		QPair<QString, QString > selectionRes = countPropertyCharacteristicForConstraintElement(constraint, resElementName.at(0).toLower() + resElementName.mid(1) + "Selection", "false"
													 , selectionList.at(0), selectionList.at(1), selectionList.at(2), curElementOfList, depth, addStr + "	");
		resString += selectionRes.first;
		resString += addStr + "	if (" + selectionRes.second + ") {\n";
	}

	if (resType == "node") {
		resString += countRealConstraintForNodeElement(constraint, curElementOfList, functionName + "Res", depth + 1, addStr + "	");
	} else if (resType == "edge") {
		resString += countRealConstraintForEdgeElement(constraint, curElementOfList, functionName + "Res", depth + 1, addStr + "	");
	}
	resString += addStr + "	main" + resElementName + "Res_" + QString::number(depth + 1) + " = main" + resElementName + "Res_" + QString::number(depth + 1) + " && " + functionName + "Res_" + QString::number(depth + 1) + ";\n";

	if (neededCount) {
		resString += addStr + "	tempCount" + resElementName + "_" + QString::number(depth) + "++;\n";
	}

	if (neededSelection) {
		resString += addStr + "	}\n";
	}

	resString += addStr + "}\n";
	resBool.push_back("main" + resElementName + "Res_" + QString::number(depth + 1));

	if (neededCount) {
		QList<QString> countList = count.split(" ");
		QString sign = countList.at(0);
		if (sign == "=") {
			sign += "=";
		}
		sign.replace("&lt;", "<");

		resString += addStr + "bool count" + resElementName + "Res_" + QString::number(depth)
				+ " = (tempCount" + resElementName + "_" + QString::number(depth) + " " + sign + " " + countList.at(1) + ");\n";
		resBool.push_back("count" + resElementName + "Res_" + QString::number(depth));
	}

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForChildrens(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resChildrensConstraint = countConstraintForListOfElements(constraint, elementName, "Children" , "children", "node", depth, addStr);
	resString = resChildrensConstraint.first;
	resBool = resChildrensConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForOutgoingLinks(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resOutgoingLinksConstraint = countConstraintForListOfElements(constraint, elementName, "OutgoingLinks" , "outgoingLinks", "edge", depth, addStr);
	resString = resOutgoingLinksConstraint.first;
	resBool = resOutgoingLinksConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForIncomingLinks(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resIncomingLinksConstraint = countConstraintForListOfElements(constraint, elementName, "IncomingLinks" , "incomingLinks", "edge", depth, addStr);
	resString = resIncomingLinksConstraint.first;
	resBool = resIncomingLinksConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForOutgoingNodes(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resOutgoingNodesConstraint = countConstraintForListOfElements(constraint, elementName, "OutgoingNodes" , "outgoingNodes", "node", depth, addStr);
	resString = resOutgoingNodesConstraint.first;
	resBool = resOutgoingNodesConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForIncomingNodes(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resIncomingNodesConstraint = countConstraintForListOfElements(constraint, elementName, "IncomingNodes" , "incomingNodes", "node", depth, addStr);
	resString = resIncomingNodesConstraint.first;
	resBool = resIncomingNodesConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QString > ConcreateGenerator::countPropertyCharacteristicForConstraintElement(Id const &constraint, QString const &characteristicName, QString const &defaultValue
													, QString property, QString sign, QString value
													, QString elementName, int depth, QString addStr)
{
	QString resString = "";

	if (sign == "=") {
		sign += "=";
	}
	sign.replace("&lt;", "<");

	resString += addStr + "bool " + characteristicName + "Res_" + QString::number(depth) + " = " + defaultValue + ";\n";

	if (property.compare("TYPE", Qt::CaseInsensitive) == 0) {
		resString += addStr + "if (" + elementName + " != qReal::Id::rootId()) {\n";
		resString += addStr + "	" + characteristicName + "Res_" + QString::number(depth) + " = (" + elementName + ".element() " + sign + " \"" + value + "\");\n";
		resString += addStr + "}\n";

	} else if (property.compare("METATYPE", Qt::CaseInsensitive) == 0) {
		resString += addStr + "if (" + elementName + " != qReal::Id::rootId()) {\n";
		resString += addStr + "	qReal::EditorManagerInterface::MetaType isNodeOrEdge = mEditorManager->metaTypeOfElement(" + elementName + ");\n";

		if (value.compare("node", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::node";
		} else if (value.compare("edge", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::edge";
		} else {
			mErrorReporter.addCritical("Metatype \"" + value + "\" is not exist. Select \"node\" or \"edge\".", constraint);
		}

		resString += addStr + "	" + characteristicName + "Res_" + QString::number(depth) + " = (isNodeOrEdge " + sign + " " + value + ");\n";
		resString += addStr + "}\n";

	} else {
		bool valueIsInt = false;
		int intValue = value.toInt(&valueIsInt);
		Q_UNUSED(intValue);

		resString += addStr + "if (logicalApi.hasProperty(" + elementName + ", \"" + property + "\")) {\n";
		resString += addStr + "	" + characteristicName + "Res_" + QString::number(depth) + " = (logicalApi.property(" + elementName + ", \"" + property + "\").";

		if (!valueIsInt) {
			resString += "toString() " + sign + " \"" + value + "\");\n";
		} else {
			resString += "toInt() " + sign + " " + value + ");\n";
		}
		resString += addStr + "}\n";
	}

	QString resBool = characteristicName + "Res_" + QString::number(depth);

	return QPair<QString, QString>(resString, resBool);
}

QPair<QString, QList<QString> > ConcreateGenerator::countConstraintForPropertyNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString property = mApi.property(constraint, "property").toString();
	QString sign = mApi.property(constraint, "sign").toString();
	QString value = mApi.property(constraint, "value").toString();
	QPair<QString, QString> res = countPropertyCharacteristicForConstraintElement(constraint, "propertyNode", "true", property, sign, value, elementName, depth, addStr);
	QList<QString> resBool;
	resBool.push_back(res.second);

	return QPair<QString, QList<QString> >(res.first, resBool);
}

QString ConcreateGenerator::additionalCommonPartForConstraint(QList<QString> resBool, QString resultName, int depth, QString addStr)
{
	QString resString = "";

	if (resBool.empty()) {
		resBool.push_back("true"); //"true";//если список детей пуст, то нормально всё : true;
								//вернуть ошибку, только если список детей НЕ пуст; т.е мы не смогли сгенерить ограничения для элемента
	}
	resString += addStr + ("	bool " + resultName + "_" + QString::number(depth) + " = " + resBool.first());
	resBool.pop_front();
	foreach (QString curBool, resBool) {
		resString += " && " + curBool;
	}
	resString += ";\n";

	return resString;
}

QString ConcreateGenerator::countRealConstraintForEdgeElement(Id const &constraintElement, QString elementName, QString resultName, int depth, QString addStr)
{
	IdList list = mApi.children(constraintElement);
	QString resString = "";
	QList<QString> resBool;

	foreach (Id const &constraint, list) {
		QString constraintType = constraint.element();
		if (!mCountsOfConstraintElementsInOneConstraint.contains(constraintType)) {
			mCountsOfConstraintElementsInOneConstraint.insert(constraintType, 0);
		}
		int additionalDepth = mCountsOfConstraintElementsInOneConstraint[constraintType];

		if (constraintType == "BeginNode") {
			QPair<QString, QList<QString> > resBeginNodeConstraint = countConstraintForBeginNode(constraint, elementName, depth + additionalDepth, addStr);
			resString += resBeginNodeConstraint.first;
			resBool.append(resBeginNodeConstraint.second);

		} else if (constraintType == "EndNode") {
			QPair<QString, QList<QString> > resEndNodeConstraint = countConstraintForEndNode(constraint, elementName, depth + additionalDepth, addStr);
			resString += resEndNodeConstraint.first;
			resBool.append(resEndNodeConstraint.second);
		} else if (constraintType == "PropertyNode") {
			QPair<QString, QList<QString> > resPropertyNodeConstraint = countConstraintForPropertyNode (constraint, elementName, depth + additionalDepth, addStr);
			resString += resPropertyNodeConstraint.first;
			resBool.append(resPropertyNodeConstraint.second);
		}

		mCountsOfConstraintElementsInOneConstraint[constraintType] ++;
	}
	resString += additionalCommonPartForConstraint(resBool, resultName, depth, addStr); //сбрасываем resBool в resString

	return resString;
}

QString ConcreateGenerator::countRealConstraintForNodeElement(Id const &constraintElement, QString elementName, QString resultName, int depth, QString addStr)
{
	IdList list = mApi.children(constraintElement);
	QString resString = "";
	QList<QString> resBool;

	foreach (Id const &constraint, list) {
		QString constraintType = constraint.element();
		if (!mCountsOfConstraintElementsInOneConstraint.contains(constraintType)) {
			mCountsOfConstraintElementsInOneConstraint.insert(constraintType, 0);
		}
		int additionalDepth = mCountsOfConstraintElementsInOneConstraint[constraintType];

		if (constraintType == "Parent") {
			QPair<QString, QList<QString> > resParentConstraint = countConstraintForParent(constraint, elementName, depth + additionalDepth, addStr);
			resString += resParentConstraint.first;
			resBool.append(resParentConstraint.second);

		} else if (constraintType == "Childrens") {
			QPair<QString, QList<QString> > resChildrensConstraint = countConstraintForChildrens (constraint, elementName, depth + additionalDepth, addStr);
			resString += resChildrensConstraint.first;
			resBool.append(resChildrensConstraint.second);

		} else if (constraintType == "OutgoingLinks") {
			QPair<QString, QList<QString> > resOutgoingLinksConstraint = countConstraintForOutgoingLinks (constraint, elementName, depth + additionalDepth, addStr);
			resString += resOutgoingLinksConstraint.first;
			resBool.append(resOutgoingLinksConstraint.second);

		} else if (constraintType == "IncomingLinks") {
			QPair<QString, QList<QString> > resIncomingLinksConstraint = countConstraintForIncomingLinks (constraint, elementName, depth + additionalDepth, addStr);
			resString += resIncomingLinksConstraint.first;
			resBool.append(resIncomingLinksConstraint.second);

		} else if (constraintType == "OutgoingNodes") {
			QPair<QString, QList<QString> > resOutgoingNodesConstraint = countConstraintForOutgoingNodes (constraint, elementName, depth + additionalDepth, addStr);
			resString += resOutgoingNodesConstraint.first;
			resBool.append(resOutgoingNodesConstraint.second);

		} else if (constraintType == "IncomingNodes") {
			QPair<QString, QList<QString> > resIncomingNodesConstraint = countConstraintForIncomingNodes (constraint, elementName, depth + additionalDepth, addStr);
			resString += resIncomingNodesConstraint.first;
			resBool.append(resIncomingNodesConstraint.second);

		} else if (constraintType == "PropertyNode") {
			QPair<QString, QList<QString> > resPropertyNodeConstraint = countConstraintForPropertyNode (constraint, elementName, depth + additionalDepth, addStr);
			resString += resPropertyNodeConstraint.first;
			resBool.append(resPropertyNodeConstraint.second);
		}

		mCountsOfConstraintElementsInOneConstraint[constraintType] ++;
	}

	resString += additionalCommonPartForConstraint(resBool, resultName, depth, addStr);

	return resString;
}
