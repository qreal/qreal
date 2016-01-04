#include "concreteGenerator.h"
#include "generatorUtils/auxiliaryMethods.h"

#include <qrutils/nameNormalizer.h>

#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace constraints::generator;
using namespace constraints::generator::generatorUtils;
using namespace qReal;
using namespace utils;

ConcreteGenerator::ConcreteGenerator(const QString &templateDirPath
		, const QString &outputDirPath
		, const QString &pathToQReal
		, const qReal::LogicalModelAssistInterface &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, const QString &metamodelLanguageName
		, const QString &constraintsMetamodelName
		)
	: AbstractGenerator(templateDirPath, outputDirPath + QString("/constraints" + constraintsMetamodelName + "/"), logicalModel, errorReporter)
	, mPathToQReal(pathToQReal)
	, mMetamodelName(metamodelLanguageName)
	, mConstraintsName(constraintsMetamodelName)
{
	mPathToQReal.replace("\\", "/");
}

ConcreteGenerator::~ConcreteGenerator()
{
}

QString ConcreteGenerator::constraintModelFullName() const
{
	return mOutputDirPath; // + "constraints" + mMetamodelName + ".pro";
}

QString ConcreteGenerator::constraintModelName() const
{
	return "constraints" + mMetamodelName;
}

QString ConcreteGenerator::constraintConstraintsModelName() const
{
	return "constraints" + mConstraintsName;
}

QString ConcreteGenerator::normalizedConstraintModelName() const
{
	return NameNormalizer::normalize(constraintModelName(), false);
}

QString ConcreteGenerator::normalizedConstraintConstraintsModelName() const
{
	return NameNormalizer::normalize(constraintConstraintsModelName(), false);
}

QString ConcreteGenerator::constraintModelId() const
{
	return mConstraintsName;
}

void ConcreteGenerator::generateCodeForProFile()
{
	QString resultPRO;

	loadTemplateFromFile(fileNamePRO, resultPRO);

	const QString hFiles = generatedPartOfTemplateForAllDiagrams("@@constraintsDiagramHFile@@");
	const QString cppFiles = generatedPartOfTemplateForAllDiagrams("@@constraintsDiagramCppFile@@");

	resultPRO.replace("@@pathToQReal@@", mPathToQReal)
			.replace("@@metamodelName@@", mMetamodelName)
			.replace("@@constraintsDiagramHFiles@@", hFiles)
			.replace("@@constraintsDiagramCppFiles@@", cppFiles);

	saveOutputFile(QString(normalizedConstraintConstraintsModelName() + ".pro"), resultPRO);
}

void ConcreteGenerator::generateCodeForConstraintsPluginHFile()
{
	QString resultPluginH;

	loadTemplateFromFile(fileNamePluginH, resultPluginH);

	const QString includeFilesPluginH = generatedPartOfTemplateForAllDiagrams("@@includeConstraintsDiagramFile@@");
	const QString privateFieldsPluginH = generatedPartOfTemplateForAllDiagrams("@@privateFieldOfConstraintsDigram@@");

	resultPluginH.replace("@@includeConstraintsDiagramFiles@@", includeFilesPluginH);
	resultPluginH.replace("@@privateFieldsOfConstraintsDigrams@@", privateFieldsPluginH);
	resultPluginH.replace("@@pathToQReal@@", mPathToQReal);

	QString fileBaseName = normalizedConstraintModelName();
	saveOutputFile(QString(fileBaseName+ "Plugin.h"), resultPluginH);
}

void ConcreteGenerator::generateCodeForConstraintsPluginCppFile()
{
	QString resultPluginCPP;

	loadTemplateFromFile(fileNamePluginCPP, resultPluginCPP);

	const QString ifForMainCheckPluginCPP =
			generatedPartOfTemplateForAllDiagrams("@@ifForMainCheckOfConstraintsDiagram@@", true);

	resultPluginCPP.replace("@@metamodelName@@", mMetamodelName);
	resultPluginCPP.replace("@@ifForMainCheckOfConstraintsDiagrams@@", ifForMainCheckPluginCPP);
	resultPluginCPP.replace("@@constraintsPluginId@@", mConstraintsName);

	QString fileBaseName = normalizedConstraintModelName();
	saveOutputFile(QString(fileBaseName + "Plugin.cpp"), resultPluginCPP);
}

QString ConcreteGenerator::templateWithReplacedDiagramName(const QString &templateName
		, const QString &diagramName) const
{
	QString templateString = mTemplateUtils[templateName];
	templateString.replace("@@diagramName@@", diagramName);

	return templateString;
}

QString ConcreteGenerator::generatedPartOfTemplateForAllDiagrams(const QString &templateName
		, const bool isIfForMainCheck)
{
	QString resultString;
	int count = 1;

	for (const Id &diagram : mApi.elementsByType("ConstraintsDiagram")) {
		if (mApi.isLogicalElement(diagram)) {
			QString diagramNameTemplate = mTemplateUtils["@@diagramName@@"];
			diagramNameTemplate = replaceLanguageName(diagramNameTemplate, diagram, count);
			diagramNameTemplate.truncate(diagramNameTemplate.size() - 1);

			QString templateForOneDiagram = templateWithReplacedDiagramName(templateName, diagramNameTemplate);

			if (isIfForMainCheck) {
				templateForOneDiagram.replace("@@keywordForAllEdges@@", keywordForAllEdges);
				templateForOneDiagram.replace("@@keywordForAllNodes@@", keywordForAllNodes);
				templateForOneDiagram.replace("@@keywordForAllLanguages@@", keywordForAllLanguages);
			}

			count++;
			resultString += templateForOneDiagram;
		}
	}

	return resultString;
}

ConcreteGenerator::NeededStringsForConcreteGenerate ConcreteGenerator::generateCommonNeededPartsForElements(QString elementName)
{
	QString mainChecksForElementsH;
	QString returnCheckStatusesOfElementsInCheckCPP;
	QString addElementsInElementsNamesCPP;

	QString mainCheckOfElementH = mTemplateUtils["@@mainCheckElement@@"];
	mainCheckOfElementH.replace("@@elementName@@", elementName);
	mainChecksForElementsH += mainCheckOfElementH;

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

	return NeededStringsForConcreteGenerate("", mainChecksForElementsH, "", "", "", returnCheckStatusesOfElementsInCheckCPP, addElementsInElementsNamesCPP);
}

QString ConcreteGenerator::generateMainCheckStatusesForElementsCPP(QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP)
{
	QString countMainCheckStatusesForElementsCPP;
	foreach (QString curElementName, appendOptionalCheckStatusInMainCheckCPP.keys()) {
		QString appendOptionalCheckStatusOfElement = appendOptionalCheckStatusInMainCheckCPP[curElementName];
		QString countMainCheckStatusOfElementCPP = mTemplateUtils["@@countMainCheckStatusOfElement@@"];
		countMainCheckStatusOfElementCPP.replace("@@elementName@@", curElementName);
		countMainCheckStatusOfElementCPP.replace("@@appendOptionalCheckStatusesOfElement@@", appendOptionalCheckStatusOfElement);

		countMainCheckStatusesForElementsCPP += countMainCheckStatusOfElementCPP;
	}
	return countMainCheckStatusesForElementsCPP;
}

ConcreteGenerator::NeededStringsForConcreteGenerate ConcreteGenerator::generateNeededPartsForDiagramFiles(Id const &diagram)
{
	QString optionalChecksForElementsH;
	QString mainChecksForElementsH;
	QString countOptionalCheckStatusesForElementsCPP;
	QString countMainCheckStatusesForElementsCPP;
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
			mErrorReporter.addCritical(QObject::tr("Name of constraintNode not found!"), element);
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
			elementCount[elementName]++;
		} else {
			elementCount.insert(elementName, 1);
		}

		int id = elementCount.value(elementName);
		if (id == 1) {
			NeededStringsForConcreteGenerate neededString = generateCommonNeededPartsForElements(elementName);
			mainChecksForElementsH += neededString.mainChecksForElementsH;
			returnCheckStatusesOfElementsInCheckCPP += neededString.returnCheckStatusesOfElementsInCheckCPP;
			addElementsInElementsNamesCPP += neededString.addElementsInElementsNamesCPP;
		}

		QString elementNameWithId = mTemplateUtils["@@elementNameWithId@@"];
		elementNameWithId.replace("@@elementName@@", elementName).replace("@@id@@", QString::number(id));
		elementNameWithId.truncate(elementNameWithId.size() - 1);

		QString optionalCheckOfElementH = mTemplateUtils["@@optionalCheckElement@@"];
		optionalCheckOfElementH.replace("@@elementNameWithId@@", elementNameWithId);
		optionalChecksForElementsH += optionalCheckOfElementH;

		QString countOptionalCheckStatusOfElement = mTemplateUtils["@@countOptionalCheckStatusOfElement@@"];
		countOptionalCheckStatusOfElement.replace("@@elementNameWithId@@", elementNameWithId);
		countOptionalCheckStatusOfElement.replace("@@countRealCheckStatusOfElement@@", countRealConstraintOfDiagramElement(element));
		countOptionalCheckStatusOfElement.replace("@@errorText@@", mApi.property(element, "errorText").toString());
		countOptionalCheckStatusOfElement.replace("@@checkStatus@@", mApi.property(element, "errorType").toString());
		countOptionalCheckStatusesForElementsCPP += countOptionalCheckStatusOfElement;

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

	countMainCheckStatusesForElementsCPP += generateMainCheckStatusesForElementsCPP(appendOptionalCheckStatusInMainCheckCPP);

	return NeededStringsForConcreteGenerate(optionalChecksForElementsH, mainChecksForElementsH, countOptionalCheckStatusesForElementsCPP, countMainCheckStatusesForElementsCPP
											, prefixForReturnCheckStatusesOfElementsInCheckCPP, returnCheckStatusesOfElementsInCheckCPP, addElementsInElementsNamesCPP);
}

void ConcreteGenerator::generateDiagramFiles(Id const &diagram, QString diagramNameTemplate)
{
	QString resultDiagramH;
	QString resultDiagramCPP;
	loadTemplateFromFile(fileNameDiagramH, resultDiagramH);
	loadTemplateFromFile(fileNameDiagramCPP, resultDiagramCPP);

	NeededStringsForConcreteGenerate needeStrings = generateNeededPartsForDiagramFiles(diagram);

	resultDiagramH.replace("@@optionalChecksForElements@@", needeStrings.optionalChecksForElementsH);
	resultDiagramH.replace("@@mainChecksForElements@@", needeStrings.mainChecksForElementsH);
	resultDiagramH.replace("@@diagramName@@", diagramNameTemplate);
	resultDiagramH.replace("@@pathToQReal@@", mPathToQReal);

	resultDiagramCPP.replace("@@countOptionalCheckStatusesForElements@@", needeStrings.countOptionalCheckStatusesForElementsCPP);
	resultDiagramCPP.replace("@@countMainCheckStatusesForElements@@", needeStrings.countMainCheckStatusesForElementsCPP);
	resultDiagramCPP.replace("@@prefixForReturnCheckStatusesOfElementsInCheck@@", needeStrings.prefixForReturnCheckStatusesOfElementsInCheckCPP);
	resultDiagramCPP.replace("@@returnCheckStatusesOfElementsInCheck@@", needeStrings.returnCheckStatusesOfElementsInCheckCPP);
	resultDiagramCPP.replace("@@addElementsInElementsNames@@", needeStrings.addElementsInElementsNamesCPP);
	resultDiagramCPP.replace("@@diagramName@@", diagramNameTemplate);
	resultDiagramCPP.replace("@@languageName@@", correctedLanguageName(diagram));

	saveOutputFile(QString("constraints" + diagramNameTemplate + ".h"), resultDiagramH);
	saveOutputFile(QString("constraints" + diagramNameTemplate + ".cpp"), resultDiagramCPP);
}

void ConcreteGenerator::generate()
{
	loadUtilsTemplates();

	generateCodeForProFile();
	generateCodeForConstraintsPluginHFile();
	generateCodeForConstraintsPluginCppFile();

	int count = 1;

	for (const Id &diagram : mApi.elementsByType("ConstraintsDiagram")) {
		if (!mApi.isLogicalElement(diagram)) {
			continue;
		}

		mUsedMetaTypeInCheck = false;

		QString diagramNameTemplate = mTemplateUtils["@@diagramName@@"];
		diagramNameTemplate = replaceLanguageName(diagramNameTemplate, diagram, count);
		diagramNameTemplate.truncate(diagramNameTemplate.size() - 1);

		generateDiagramFiles(diagram, diagramNameTemplate);

		count++;
	}
}

QString ConcreteGenerator::correctedLanguageName(Id const &diagram)
{
	QString languageName = mApi.property(diagram, "languageName").toString();
	if (languageName.isEmpty()) {
		mErrorReporter.addCritical(QObject::tr("LanguageName of conatraints diagram not found."), diagram);
	}
	if ((languageName.compare("all", Qt::CaseInsensitive) == 0) || (languageName.compare(keywordForAllLanguages, Qt::CaseInsensitive) == 0)) {
		languageName = keywordForAllLanguages;
	}
	return languageName;
}

QString ConcreteGenerator::replaceLanguageName(QString string, Id const &diagram, int count)
{
	string.replace("@@languageName@@", correctedLanguageName(diagram))
			.replace("@@id@@", QString::number(count));
	return string;
}

QPair<bool, QString> ConcreteGenerator::handleConstraintsSelection(Id const &constraintElement)
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

QString ConcreteGenerator::countRealConstraintOfDiagramElement(Id const &constraintElement)
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

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForBeginNode(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

	resString += addStr + "qReal::Id newBeginNodeName_" + QString::number(depth) + " = logicalApi.from(" + elementName + ");\n";
	resString += countRealConstraintForNodeElement(constraint, "newBeginNodeName_" + QString::number(depth) , "beginNodeRes", depth + 1, addStr);
	resBool.push_back("beginNodeRes_" + QString::number(depth + 1));

	resString += AuxiliaryMethods::generateExistsProperty("beginNodeRes", "newBeginNodeName", constraint, depth, addStr, mApi);
	resBool.push_back("beginNodeRes_" + QString::number(depth));

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForEndNode(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

	resString += addStr + "qReal::Id newEndNodeName_" + QString::number(depth) + " = logicalApi.to(" + elementName + ");\n";
	resString += countRealConstraintForNodeElement(constraint, "newEndNodeName_" + QString::number(depth), "endNodeRes", depth + 1, addStr);
	resBool.push_back("endNodeRes_" + QString::number(depth + 1));

	resString += AuxiliaryMethods::generateExistsProperty("endNodeRes", "newEndNodeName", constraint, depth, addStr, mApi);
	resBool.push_back("endNodeRes_" + QString::number(depth));

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForParent(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

	resString += addStr + "qReal::Id newParentName_" + QString::number(depth) + " = logicalApi.parent(" + elementName + ");\n";
	resString += addStr + "bool mainParentRes_" + QString::number(depth) + " = true;\n";
	resString += addStr + "if (newParentName_" + QString::number(depth) + " != qReal::Id::rootId()) {\n";
	resString += addStr + countRealConstraintForNodeElement(constraint, "newParentName_" + QString::number(depth) , "parentNodeRes", depth + 1, addStr + "	");
	resString += addStr + "	mainParentRes_" + QString::number(depth) + " = parentNodeRes_" + QString::number(depth + 1) + ";\n";
	resString += addStr + "	}\n";
	resBool.push_back("mainParentRes_" + QString::number(depth));

	resString += AuxiliaryMethods::generateExistsProperty("parentRes", "newParentName", constraint, depth, addStr, mApi);
	resBool.push_back("parentRes_" + QString::number(depth));

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForListOfElements(Id const &constraint, QString elementName, QString resElementName, QString functionName, QString resType, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

	QString count = mApi.property(constraint, "count").toString();
	bool neededCount  = (!count.isEmpty());

	if (neededCount) {
		if (QRegExp("(([<>=]|&lt;|&rt;)=?)\\s*(\\d+)").exactMatch(count)) {
			count.replace(QRegExp("([<>=&rlt;]+)(\\d+)"), "\\1 \\2");
		} else {
			mErrorReporter.addCritical(QObject::tr("Value \"%1\" is not correct for property 'Count'.").arg(count), constraint);
			neededCount = false;
		}
	}

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

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForChildrens(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resChildrensConstraint = countConstraintForListOfElements(constraint, elementName, "Children" , "children", "node", depth, addStr);
	resString = resChildrensConstraint.first;
	resBool = resChildrensConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForOutgoingLinks(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resOutgoingLinksConstraint = countConstraintForListOfElements(constraint, elementName, "OutgoingLinks" , "outgoingLinks", "edge", depth, addStr);
	resString = resOutgoingLinksConstraint.first;
	resBool = resOutgoingLinksConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForIncomingLinks(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resIncomingLinksConstraint = countConstraintForListOfElements(constraint, elementName, "IncomingLinks" , "incomingLinks", "edge", depth, addStr);
	resString = resIncomingLinksConstraint.first;
	resBool = resIncomingLinksConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForOutgoingNodes(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resOutgoingNodesConstraint = countConstraintForListOfElements(constraint, elementName, "OutgoingNodes" , "outgoingNodes", "node", depth, addStr);
	resString = resOutgoingNodesConstraint.first;
	resBool = resOutgoingNodesConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForIncomingNodes(Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	QPair<QString, QList<QString> > resIncomingNodesConstraint = countConstraintForListOfElements(constraint, elementName, "IncomingNodes" , "incomingNodes", "node", depth, addStr);
	resString = resIncomingNodesConstraint.first;
	resBool = resIncomingNodesConstraint.second;

	return QPair<QString, QList<QString> >(resString, resBool);
}

QPair<QString, QString > ConcreteGenerator::countPropertyCharacteristicForConstraintElement(Id const &constraint, QString const &characteristicName, QString const &defaultValue
													, QString property, QString sign, QString value
													, QString elementName, int depth, QString addStr)
{
	QString resString = "";

	if (sign == "=") {
		sign += "=";
	}
	sign.replace("&lt;", "<");

	resString += addStr + "bool " + characteristicName + "Res_" + QString::number(depth) + " = " + defaultValue + ";\n";

	if (property.compare("TYPE", Qt::CaseSensitive) == 0) {
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
			mErrorReporter.addCritical(QObject::tr("Metatype \"%1\" is not exist. Select \"node\" or \"edge\".").arg(value), constraint);
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

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForPropertyNode(qReal::Id const &constraint, QString elementName, int depth, QString addStr)
{
	QString property = mApi.property(constraint, "property").toString();
	QString sign = mApi.property(constraint, "sign").toString();
	QString value = mApi.property(constraint, "value").toString();
	QPair<QString, QString> res = countPropertyCharacteristicForConstraintElement(constraint, "propertyNode", "true", property, sign, value, elementName, depth, addStr);
	QList<QString> resBool;
	resBool.push_back(res.second);

	return QPair<QString, QList<QString> >(res.first, resBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForMultiOrNode(Id const &constraint, IdList &usedElements, metaType const &type, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> curBool;
	QList<QString> resBool;

	IdList neighborNodes = AuxiliaryMethods::neighborNodesWithGivenType(constraint, "MultiOrEdge", mApi);
	usedElements.append(neighborNodes);
	foreach (Id element, neighborNodes) {
		QPair<QString, QList<QString> > resElementConstraint;
		if (type == edge) {
			resElementConstraint = countRealConstraintForOneEdgeElement(element, usedElements, elementName, depth + 1, addStr, true);
		} else if (type == node) {
			resElementConstraint = countRealConstraintForOneNodeElement(element, usedElements, elementName, depth + 1, addStr, true);
		}

		resString += resElementConstraint.first;
		curBool.append(resElementConstraint.second);
	}
	resBool.append(QString("(" + AuxiliaryMethods::disjunctionExpression(curBool) + ")"));

	return QPair<QString, QList<QString> >(resString, resBool);
}

QString ConcreteGenerator::additionalCommonPartForConstraint(QList<QString> resBool, QString resultName, int depth, QString addStr)
{
	QString resString = "";

	resString += addStr + ("	bool " + resultName + "_" + QString::number(depth) + " = " + AuxiliaryMethods::conjunctionExpression(resBool));
	resString += ";\n";

	return resString;
}

QPair<QString, QList<QString> > ConcreteGenerator::countNeighborsElementsByOr(Id const &constraint, QString resConstraintBool, IdList &usedElements, metaType const &type, QString elementName, int depth, QString addStr)
{
	QString resString = "";
	QList<QString> resBool;

	IdList neighborNodes = AuxiliaryMethods::neighborNodesWithGivenType(constraint, "Or", mApi);
	if (!neighborNodes.isEmpty()) {
		foreach (Id neighbor, neighborNodes) {
			if (!usedElements.contains(neighbor)) {
				usedElements.append(neighbor);
				QPair<QString, QList<QString> > resNeighborsConstraint;
				if (type == edge) {
					resNeighborsConstraint = countRealConstraintForOneEdgeElement(neighbor, usedElements, elementName, depth, addStr);
				} else if (type == node) {
					resNeighborsConstraint = countRealConstraintForOneNodeElement(neighbor, usedElements, elementName, depth, addStr);
				}

				resString += resNeighborsConstraint.first;
				resBool.append(QString("( " + resConstraintBool + " || " + AuxiliaryMethods::conjunctionExpression(resNeighborsConstraint.second) + " )"));
			}
		}
	}

	return QPair<QString, QList<QString> >(resString, resBool);
}

QString ConcreteGenerator::countRealConstraintForElement(Id const &constraintElement, metaType const &type, QString elementName, QString resultName, int depth, QString addStr)
{
	IdList list = mApi.children(constraintElement);
	QString resString = "";
	QList<QString> resBool;
	IdList usedElements;

	foreach (Id const &constraint, list) {
		if (!usedElements.contains(constraint)) {
			QPair<QString, QList<QString> > resOneConstraint;
			if (type == edge) {
				resOneConstraint = countRealConstraintForOneEdgeElement(constraint, usedElements, elementName, depth, addStr);
			} else if (type == node) {
				resOneConstraint = countRealConstraintForOneNodeElement(constraint, usedElements, elementName, depth, addStr);
			}
			resString += resOneConstraint.first;
			resBool.append(resOneConstraint.second);
		}
	}
	resString += additionalCommonPartForConstraint(resBool, resultName, depth, addStr); //сбрасываем resBool в resString

	return resString;
}

QString ConcreteGenerator::countRealConstraintForEdgeElement(Id const &constraintElement, QString elementName, QString resultName, int depth, QString addStr)
{
	return countRealConstraintForElement(constraintElement, edge, elementName, resultName, depth, addStr);
}

QString ConcreteGenerator::countRealConstraintForNodeElement(Id const &constraintElement, QString elementName, QString resultName, int depth, QString addStr)
{
	return countRealConstraintForElement(constraintElement, node, elementName, resultName, depth, addStr);
}

QPair<QString, QList<QString> > ConcreteGenerator::countRealConstraintForOneEdgeElement(Id const &constraint, IdList &usedElements, QString elementName, int depth, QString addStr, bool isMultiOr)
{
	QString resString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

	QString constraintType = constraint.element();
	if (!mCountsOfConstraintElementsInOneConstraint.contains(constraintType)) {
		mCountsOfConstraintElementsInOneConstraint.insert(constraintType, 0);
	}
	int additionalDepth = mCountsOfConstraintElementsInOneConstraint[constraintType];

	if ((!AuxiliaryMethods::linkWithGivenTypeExists(constraint, "MultiOrEdge", mApi)) || isMultiOr) {
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
	} else if (constraintType == "MultiOrNode") {
		QPair<QString, QList<QString> > resMultiOrNodeConstraint = countConstraintForMultiOrNode (constraint, usedElements, edge, elementName, depth + additionalDepth, addStr);
		resString += resMultiOrNodeConstraint.first;
		resBool.append(resMultiOrNodeConstraint.second);
	}

	mCountsOfConstraintElementsInOneConstraint[constraintType] ++;

	QPair<QString, QList<QString> > resNeighborsNodes = countNeighborsElementsByOr(constraint, AuxiliaryMethods::conjunctionExpression(resBool), usedElements, edge, elementName, depth + additionalDepth, addStr);
	resString += resNeighborsNodes.first;
	if (!resNeighborsNodes.second.isEmpty()) {
		resBool.append(resNeighborsNodes.second);
	}

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countRealConstraintForOneNodeElement(Id const &constraint, IdList &usedElements, QString elementName, int depth, QString addStr, bool isMultiOr)
{
	QString resString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

	QString constraintType = constraint.element();
	if (!mCountsOfConstraintElementsInOneConstraint.contains(constraintType)) {
		mCountsOfConstraintElementsInOneConstraint.insert(constraintType, 0);
	}
	int additionalDepth = mCountsOfConstraintElementsInOneConstraint[constraintType];

	if ((!AuxiliaryMethods::linkWithGivenTypeExists(constraint, "MultiOrEdge", mApi)) || isMultiOr) {
		if (constraintType == "Parent") {
			QPair<QString, QList<QString> > resParentConstraint =
					countConstraintForParent(constraint, elementName, depth + additionalDepth, addStr);
			resString += resParentConstraint.first;
			resBool.append(resParentConstraint.second);
		} else if (constraintType == "Childrens") {
			QPair<QString, QList<QString> > resChildrensConstraint =
					countConstraintForChildrens(constraint, elementName, depth + additionalDepth, addStr);
			resString += resChildrensConstraint.first;
			resBool.append(resChildrensConstraint.second);

		} else if (constraintType == "OutgoingLinks") {
			QPair<QString, QList<QString> > resOutgoingLinksConstraint =
					countConstraintForOutgoingLinks(constraint, elementName, depth + additionalDepth, addStr);
			resString += resOutgoingLinksConstraint.first;
			resBool.append(resOutgoingLinksConstraint.second);

		} else if (constraintType == "IncomingLinks") {
			QPair<QString, QList<QString> > resIncomingLinksConstraint =
					countConstraintForIncomingLinks(constraint, elementName, depth + additionalDepth, addStr);
			resString += resIncomingLinksConstraint.first;
			resBool.append(resIncomingLinksConstraint.second);

		} else if (constraintType == "OutgoingNodes") {
			QPair<QString, QList<QString> > resOutgoingNodesConstraint =
					countConstraintForOutgoingNodes(constraint, elementName, depth + additionalDepth, addStr);
			resString += resOutgoingNodesConstraint.first;
			resBool.append(resOutgoingNodesConstraint.second);

		} else if (constraintType == "IncomingNodes") {
			QPair<QString, QList<QString> > resIncomingNodesConstraint =
					countConstraintForIncomingNodes(constraint, elementName, depth + additionalDepth, addStr);
			resString += resIncomingNodesConstraint.first;
			resBool.append(resIncomingNodesConstraint.second);

		} else if (constraintType == "PropertyNode") {
			QPair<QString, QList<QString> > resPropertyNodeConstraint =
					countConstraintForPropertyNode(constraint, elementName, depth + additionalDepth, addStr);
			resString += resPropertyNodeConstraint.first;
			resBool.append(resPropertyNodeConstraint.second);

		}
	} else if (constraintType == "MultiOrNode") {
		QPair<QString, QList<QString> > resMultiOrNodeConstraint =
				countConstraintForMultiOrNode(constraint, usedElements, node, elementName, depth + additionalDepth, addStr);
		resString += resMultiOrNodeConstraint.first;
		resBool.append(resMultiOrNodeConstraint.second);
	}

	mCountsOfConstraintElementsInOneConstraint[constraintType] ++;

	QPair<QString, QList<QString> > resNeighborsNodes = countNeighborsElementsByOr(constraint
			, AuxiliaryMethods::conjunctionExpression(resBool), usedElements, node, elementName, depth + additionalDepth, addStr);
	resString += resNeighborsNodes.first;
	if (!resNeighborsNodes.second.isEmpty()) {
		resBool = resNeighborsNodes.second;
	}

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resString, allResultBool);
}
