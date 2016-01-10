#include "concreteGenerator.h"
#include "generatorUtils/auxiliaryMethods.h"
#include "generatorParts/generatorForListsOfElements.h"

#include <qrutils/nameNormalizer.h>

#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace constraints::generator;
using namespace constraints::generator::generatorUtils;
using namespace constraints::generator::generatorParts;
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
	: AbstractGenerator(templateDirPath
			, outputDirPath + QString("/constraints" + constraintsMetamodelName + "/")
			, logicalModel
			, errorReporter)
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

ConcreteGenerator::NeededStringsForConcreteGenerate ConcreteGenerator::generateCommonNeededPartsForElements(
		const QString &elementName)
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

	return NeededStringsForConcreteGenerate("", mainChecksForElementsH, "", "", ""
			, returnCheckStatusesOfElementsInCheckCPP, addElementsInElementsNamesCPP);
}

QString ConcreteGenerator::generateMainCheckStatusesForElementsCPP(
		QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP)
{
	QString countMainCheckStatusesForElementsCPP;

	for (const QString &curElementName : appendOptionalCheckStatusInMainCheckCPP.keys()) {
		QString appendOptionalCheckStatusOfElement = appendOptionalCheckStatusInMainCheckCPP[curElementName];
		QString countMainCheckStatusOfElementCPP = mTemplateUtils["@@countMainCheckStatusOfElement@@"];
		countMainCheckStatusOfElementCPP.replace("@@elementName@@", curElementName);
		countMainCheckStatusOfElementCPP.replace("@@appendOptionalCheckStatusesOfElement@@"
				, appendOptionalCheckStatusOfElement);

		countMainCheckStatusesForElementsCPP += countMainCheckStatusOfElementCPP;
	}

	return countMainCheckStatusesForElementsCPP;
}

ConcreteGenerator::NeededStringsForConcreteGenerate ConcreteGenerator::generateNeededPartsForDiagramFiles(
		const Id &diagram)
{
	QString optionalChecksForElementsH;
	QString mainChecksForElementsH;
	QString countOptionalCheckStatusesForElementsCPP;
	QString countMainCheckStatusesForElementsCPP;
	QString returnCheckStatusesOfElementsInCheckCPP;
	QString addElementsInElementsNamesCPP;

	QMap<QString, QString> appendOptionalCheckStatusInMainCheckCPP;

	QMap<QString, int> elementCount;
	QList<Id> elementList= mApi.elementsByType("NodeConstraint");
	elementList.append(mApi.elementsByType("EdgeConstraint"));
	elementList.append(mApi.elementsByType("NodesConstraint"));
	elementList.append(mApi.elementsByType("EdgesConstraint"));

	for (const Id &element : elementList) {
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
			elementName = keywordForAllEdges;
		}

		if (elementName == "(Nodes Constraint)"
				|| (element.element() == "NodesConstraint" && ((elementName.compare("all", Qt::CaseInsensitive)) == 0)
				&& ((elementName.compare("allnodes", Qt::CaseInsensitive)) == 0)) ) {
			elementName = keywordForAllNodes;
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
		countOptionalCheckStatusOfElement.replace("@@countRealCheckStatusOfElement@@"
				, countRealConstraintOfDiagramElement(element));
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
		prefixForReturnCheckStatusesOfElementsInCheckCPP =
				mTemplateUtils["@@prefixForReturnCheckStatusOfElementByMetaTypeInCheck@@"];
	}

	countMainCheckStatusesForElementsCPP += generateMainCheckStatusesForElementsCPP(
				appendOptionalCheckStatusInMainCheckCPP);

	return NeededStringsForConcreteGenerate(optionalChecksForElementsH
			, mainChecksForElementsH
			, countOptionalCheckStatusesForElementsCPP
			, countMainCheckStatusesForElementsCPP
			, prefixForReturnCheckStatusesOfElementsInCheckCPP
			, returnCheckStatusesOfElementsInCheckCPP
			, addElementsInElementsNamesCPP);
}

void ConcreteGenerator::generateDiagramFiles(const Id &diagram, const QString &diagramNameTemplate)
{
	QString resultDiagramH;
	QString resultDiagramCPP;

	loadTemplateFromFile(fileNameDiagramH, resultDiagramH);
	loadTemplateFromFile(fileNameDiagramCPP, resultDiagramCPP);

	NeededStringsForConcreteGenerate neededStrings = generateNeededPartsForDiagramFiles(diagram);

	resultDiagramH.replace("@@optionalChecksForElements@@", neededStrings.optionalChecksForElementsH);
	resultDiagramH.replace("@@mainChecksForElements@@", neededStrings.mainChecksForElementsH);
	resultDiagramH.replace("@@diagramName@@", diagramNameTemplate);
	resultDiagramH.replace("@@pathToQReal@@", mPathToQReal);

	resultDiagramCPP.replace("@@countOptionalCheckStatusesForElements@@"
			, neededStrings.countOptionalCheckStatusesForElementsCPP);
	resultDiagramCPP.replace("@@countMainCheckStatusesForElements@@"
			, neededStrings.countMainCheckStatusesForElementsCPP);
	resultDiagramCPP.replace("@@prefixForReturnCheckStatusesOfElementsInCheck@@"
			, neededStrings.prefixForReturnCheckStatusesOfElementsInCheckCPP);
	resultDiagramCPP.replace("@@returnCheckStatusesOfElementsInCheck@@"
			, neededStrings.returnCheckStatusesOfElementsInCheckCPP);
	resultDiagramCPP.replace("@@addElementsInElementsNames@@"
			, neededStrings.addElementsInElementsNamesCPP);
	resultDiagramCPP.replace("@@diagramName@@"
			, diagramNameTemplate);
	resultDiagramCPP.replace("@@languageName@@"
			, correctedLanguageName(diagram));

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

QString ConcreteGenerator::correctedLanguageName(const Id &diagram)
{
	QString languageName = mApi.property(diagram, "languageName").toString();

	if (languageName.isEmpty()) {
		mErrorReporter.addCritical(QObject::tr("LanguageName of conatraints diagram not found."), diagram);
	}

	if ((languageName.compare("all", Qt::CaseInsensitive) == 0)
			|| (languageName.compare(keywordForAllLanguages, Qt::CaseInsensitive) == 0)) {
		languageName = keywordForAllLanguages;
	}

	return languageName;
}

QString ConcreteGenerator::replaceLanguageName(QString string, const Id &diagram, const int count)
{
	string.replace("@@languageName@@", correctedLanguageName(diagram))
			.replace("@@id@@", QString::number(count));

	return string;
}

QPair<bool, QString> ConcreteGenerator::handleConstraintsSelection(const Id &constraintElement)
{
	QString constraintElementType = constraintElement.element();

	if (constraintElementType == "EdgesConstraint" || constraintElementType == "NodesConstraint") {
		QString resultString = "";
		QList<QString> resBool;

		bool neededSelectionByProperty  = false;

		if (mApi.hasProperty(constraintElement, "selection")) {
			const QString selection = mApi.property(constraintElement, "selection").toString();
			neededSelectionByProperty  = (!selection.isEmpty()) && (selection.compare("all", Qt::CaseInsensitive) != 0);
			if (neededSelectionByProperty) {
				QList<QString> selectionList = selection.split(" ");
				QPair<QString, QString > selectionResByProperty = countPropertyCharacteristicForConstraintElement(
						constraintElement
						, "elementsSelectionByProperty"
						, "false"
						, selectionList.at(0)
						, selectionList.at(1)
						, selectionList.at(2)
						, "element"
						, 1
						, "	");
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

QString ConcreteGenerator::countRealConstraintOfDiagramElement(const Id &constraintElement)
{
	QString resultString = "";

	for (const QString &nameOfConstrainElement : mCountsOfConstraintElementsInOneConstraint.keys()) {
		mCountsOfConstraintElementsInOneConstraint[nameOfConstrainElement] = 0;
	}

	QPair<bool, QString> selectionResult = handleConstraintsSelection(constraintElement);
	if (selectionResult.first) {
		resultString += selectionResult.second;
	}

	if (constraintElement.element() == "EdgeConstraint") {
		resultString += countRealConstraintForEdgeElement(constraintElement, "element", "edgeRes", 1, "	");
		resultString += "	res = edgeRes_1;\n";

	} else if (constraintElement.element() == "NodeConstraint") {
		resultString += countRealConstraintForNodeElement(constraintElement, "element", "nodeRes", 1, "	");
		resultString += "	res = nodeRes_1;\n";

	} else if (constraintElement.element() == "EdgesConstraint") {
		resultString += countRealConstraintForEdgeElement(constraintElement, "element", "allEdgesRes", 1, "		");
		resultString += "	res = allEdgesRes_1;\n";

	} else if (constraintElement.element() == "NodesConstraint") {
		resultString += countRealConstraintForNodeElement(constraintElement, "element", "allNodesRes", 1, "		");
		resultString += "	res = allNodesRes_1;\n";
	}

	if (selectionResult.first) {
		resultString += "	}\n";
	}

	return resultString;
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForBeginNode(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString)
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
			, additionalString);

	resBool.push_back("beginNodeRes_" + QString::number(depth + 1));

	resultString += AuxiliaryMethods::generateExistsProperty("beginNodeRes"
			, "newBeginNodeName"
			, constraint
			, depth
			, additionalString
			, mApi);

	resBool.push_back("beginNodeRes_" + QString::number(depth));

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resultString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForEndNode(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString)
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
			, additionalString);

	resBool.push_back("endNodeRes_" + QString::number(depth + 1));

	resultString += AuxiliaryMethods::generateExistsProperty("endNodeRes"
			, "newEndNodeName"
			, constraint
			, depth
			, additionalString
			, mApi);

	resBool.push_back("endNodeRes_" + QString::number(depth));

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));
	return QPair<QString, QList<QString> >(resultString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForParent(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString)
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
			, additionalString + "	");

	resultString += additionalString + realConstraintForNodeElement;

	resultString += QString("%1	mainParentRes_%2 = parentNodeRes_%3;\n"
						 "%1	}\n")
			.arg(additionalString)
			.arg(depth)
			.arg(depth + 1);

	resultBool.push_back("mainParentRes_" + QString::number(depth));

	resultString += AuxiliaryMethods::generateExistsProperty(
			"parentRes"
			, "newParentName"
			, constraint
			, depth
			, additionalString
			, mApi);

	resultBool.push_back("parentRes_" + QString::number(depth));

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resultBool));

	return QPair<QString, QList<QString> >(resultString, allResultBool);
}

QPair<QString, QString > ConcreteGenerator::countPropertyCharacteristicForConstraintElement(
		const Id &constraint
		, const QString &characteristicName
		, const QString &defaultValue
		, const QString &property
		, QString sign
		, QString value
		, const QString &elementName
		, const int depth
		, const QString &addStr)
{
	QString resString = "";

	if (sign == "=") {
		sign += "=";
	}

	sign.replace("&lt;", "<");

	resString += QString("%1bool %2Res_%3 = %4;\n")
			.arg(addStr)
			.arg(characteristicName)
			.arg(depth)
			.arg(defaultValue);

	if (property.compare("TYPE", Qt::CaseSensitive) == 0) {
		resString += QString("%1if (%2 != qReal::Id::rootId()) {\n"
				"%1	%3Res_%4 = (%2.element() %5 \"%6\");\n"
				"%1}\n")
				.arg(addStr)
				.arg(elementName)
				.arg(characteristicName)
				.arg(depth)
				.arg(sign)
				.arg(value);
	} else if (property.compare("METATYPE", Qt::CaseInsensitive) == 0) {
		resString += QString("%1if (%2 != qReal::Id::rootId()) {\n"
				"%1	qReal::EditorManagerInterface::MetaType isNodeOrEdge = mEditorManager->metaTypeOfElement(%2);\n")
				.arg(addStr)
				.arg(elementName);

		if (value.compare("node", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::node";
		} else if (value.compare("edge", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::edge";
		} else {
			mErrorReporter.addCritical(QObject::tr("Metatype \"%1\" does not exist. Select \"node\" or \"edge\".")
					.arg(value), constraint);
		}

		resString += QString("%1	%2Res_%3 = (isNodeOrEdge %4 %5);\n"
				"%1}\n")
				.arg(addStr)
				.arg(characteristicName)
				.arg(depth)
				.arg(sign)
				.arg(value);
	} else {
		bool valueIsInt = false;
		int intValue = value.toInt(&valueIsInt);
		Q_UNUSED(intValue);

		resString += QString("%1if (logicalApi.hasProperty(%2, \"%3\")) {\n"
				"%1	%4Res_%5 = (logicalApi.property(%2, \"%3\").")
				.arg(addStr)
				.arg(elementName)
				.arg(property)
				.arg(characteristicName)
				.arg(depth);

		if (!valueIsInt) {
			resString += QString("toString() %1  \"%2\");\n")
					.arg(sign)
					.arg(value);
		} else {
			resString += QString("toInt() %1 %2);\n")
					.arg(sign)
					.arg(value);
		}

		resString += addStr + "}\n";
	}

	const QString resBool = QString("%1Res_%2")
			.arg(characteristicName)
			.arg(depth);

	return QPair<QString, QString>(resString, resBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForPropertyNode(
		const qReal::Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &addStr)
{
	const QString property = mApi.property(constraint, "property").toString();
	const QString sign = mApi.property(constraint, "sign").toString();
	const QString value = mApi.property(constraint, "value").toString();

	QPair<QString, QString> res = countPropertyCharacteristicForConstraintElement(
			constraint
			, "propertyNode"
			, "true"
			, property
			, sign
			, value
			, elementName
			, depth
			, addStr);

	QList<QString> resBool;
	resBool.push_back(res.second);

	return QPair<QString, QList<QString> >(res.first, resBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countConstraintForMultiOrNode(
		const Id &constraint
		, IdList &usedElements
		, const metaType &type
		, const QString &elementName
		, const int depth
		, const QString &addStr)
{
	QString resString = "";
	QList<QString> curBool;
	QList<QString> resBool;

	IdList neighborNodes = AuxiliaryMethods::neighborNodesWithGivenType(constraint, "MultiOrEdge", mApi);
	usedElements.append(neighborNodes);

	for (const Id element : neighborNodes) {
		QPair<QString, QList<QString> > resElementConstraint;
		if (type == edge) {
			resElementConstraint = countRealConstraintForOneEdgeElement(
						element
						, usedElements
						, elementName
						, depth + 1
						, addStr
						, true);
		} else if (type == node) {
			resElementConstraint = countRealConstraintForOneNodeElement(
						element
						, usedElements
						, elementName
						, depth + 1
						, addStr
						, true);
		}

		resString += resElementConstraint.first;
		curBool.append(resElementConstraint.second);
	}

	resBool.append(QString("(" + AuxiliaryMethods::disjunctionExpression(curBool) + ")"));

	return QPair<QString, QList<QString> >(resString, resBool);
}

QString ConcreteGenerator::additionalCommonPartForConstraint(
		const QList<QString> &resBool
		, const QString &resultName
		, const int depth
		, const QString &addStr)
{
	QString resString = "";

	resString += QString("%1	bool %2_%3 = %4;\n")
			.arg(addStr)
			.arg(resultName)
			.arg(depth)
			.arg(AuxiliaryMethods::conjunctionExpression(resBool));

	return resString;
}

QPair<QString, QList<QString> > ConcreteGenerator::countNeighborsElementsByOr(
		const Id &constraint
		, const QString &resConstraintBool
		, IdList &usedElements
		, const metaType &type
		, const QString &elementName
		, const int depth
		, const QString &addStr)
{
	QString resString = "";
	QList<QString> resBool;

	IdList neighborNodes = AuxiliaryMethods::neighborNodesWithGivenType(constraint, "Or", mApi);

	if (!neighborNodes.isEmpty()) {
		for (const Id neighbor : neighborNodes) {
			if (!usedElements.contains(neighbor)) {
				usedElements.append(neighbor);
				QPair<QString, QList<QString> > resNeighborsConstraint;

				if (type == edge) {
					resNeighborsConstraint = countRealConstraintForOneEdgeElement(
							neighbor
							, usedElements
							, elementName
							, depth
							, addStr);
				} else if (type == node) {
					resNeighborsConstraint = countRealConstraintForOneNodeElement(
							neighbor
							, usedElements
							, elementName
							, depth
							, addStr);
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

QString ConcreteGenerator::countRealConstraintForElement(
		const Id &constraintElement
		, const metaType &type
		, const QString &elementName
		, const QString &resultName
		, const int depth
		, const QString &addStr)
{
	IdList list = mApi.children(constraintElement);
	QString resString = "";
	QList<QString> resBool;
	IdList usedElements;

	for (const Id &constraint : list) {
		if (!usedElements.contains(constraint)) {
			QPair<QString, QList<QString> > resOneConstraint;
			if (type == edge) {
				resOneConstraint = countRealConstraintForOneEdgeElement(
						constraint
						, usedElements
						, elementName
						, depth
						, addStr);
			} else if (type == node) {
				resOneConstraint = countRealConstraintForOneNodeElement(
						constraint
						, usedElements
						, elementName
						, depth
						, addStr);
			}

			resString += resOneConstraint.first;
			resBool.append(resOneConstraint.second);
		}
	}

	resString += additionalCommonPartForConstraint(resBool, resultName, depth, addStr); //сбрасываем resBool в resString

	return resString;
}

QString ConcreteGenerator::countRealConstraintForEdgeElement(
		const Id &constraintElement
		, const QString &elementName
		, const QString &resultName
		, const int depth
		, const QString &addStr)
{
	return countRealConstraintForElement(constraintElement, edge, elementName, resultName, depth, addStr);
}

QString ConcreteGenerator::countRealConstraintForNodeElement(
		const Id &constraintElement
		, const QString &elementName
		, const QString &resultName
		, const int depth
		, const QString &addStr)
{
	return countRealConstraintForElement(constraintElement, node, elementName, resultName, depth, addStr);
}

QPair<QString, QList<QString> > ConcreteGenerator::countRealConstraintForOneEdgeElement(
		const Id &constraint
		, IdList &usedElements
		, const QString &elementName
		, const int depth
		, const QString &addStr
		, const bool isMultiOr)
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
			QPair<QString, QList<QString> > resBeginNodeConstraint = countConstraintForBeginNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr);

			resString += resBeginNodeConstraint.first;
			resBool.append(resBeginNodeConstraint.second);

		} else if (constraintType == "EndNode") {
			QPair<QString, QList<QString> > resEndNodeConstraint = countConstraintForEndNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr);

			resString += resEndNodeConstraint.first;
			resBool.append(resEndNodeConstraint.second);

		} else if (constraintType == "PropertyNode") {
			QPair<QString, QList<QString> > resPropertyNodeConstraint = countConstraintForPropertyNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr);

			resString += resPropertyNodeConstraint.first;
			resBool.append(resPropertyNodeConstraint.second);
		}
	} else if (constraintType == "MultiOrNode") {
		QPair<QString, QList<QString> > resMultiOrNodeConstraint = countConstraintForMultiOrNode(
				constraint
				, usedElements
				, edge
				, elementName
				, depth + additionalDepth
				, addStr);

		resString += resMultiOrNodeConstraint.first;
		resBool.append(resMultiOrNodeConstraint.second);
	}

	mCountsOfConstraintElementsInOneConstraint[constraintType]++;

	QPair<QString, QList<QString> > resNeighborsNodes = countNeighborsElementsByOr(
			constraint
			, AuxiliaryMethods::conjunctionExpression(resBool)
			, usedElements
			, edge
			, elementName
			, depth + additionalDepth
			, addStr);

	resString += resNeighborsNodes.first;

	if (!resNeighborsNodes.second.isEmpty()) {
		resBool.append(resNeighborsNodes.second);
	}

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));

	return QPair<QString, QList<QString> >(resString, allResultBool);
}

QPair<QString, QList<QString> > ConcreteGenerator::countRealConstraintForOneNodeElement(
		const Id &constraint
		, IdList &usedElements
		, const QString &elementName
		, const int depth
		, const QString &addStr
		, const bool isMultiOr)
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
					GeneratorForListsOfElements::countConstraintForChildrens(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, mApi
					, mErrorReporter);

			resString += resChildrensConstraint.first;
			resBool.append(resChildrensConstraint.second);
		} else if (constraintType == "OutgoingLinks") {
			QPair<QString, QList<QString> > resOutgoingLinksConstraint =
					GeneratorForListsOfElements::countConstraintForOutgoingLinks(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, mApi
					, mErrorReporter);

			resString += resOutgoingLinksConstraint.first;
			resBool.append(resOutgoingLinksConstraint.second);
		} else if (constraintType == "IncomingLinks") {
			QPair<QString, QList<QString> > resIncomingLinksConstraint =
					GeneratorForListsOfElements::countConstraintForIncomingLinks(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, mApi
					, mErrorReporter);

			resString += resIncomingLinksConstraint.first;
			resBool.append(resIncomingLinksConstraint.second);
		} else if (constraintType == "OutgoingNodes") {
			QPair<QString, QList<QString> > resOutgoingNodesConstraint =
					GeneratorForListsOfElements::countConstraintForOutgoingNodes(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, mApi
					, mErrorReporter);

			resString += resOutgoingNodesConstraint.first;
			resBool.append(resOutgoingNodesConstraint.second);
		} else if (constraintType == "IncomingNodes") {
			QPair<QString, QList<QString> > resIncomingNodesConstraint =
					GeneratorForListsOfElements::countConstraintForIncomingNodes(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr
					, mApi
					, mErrorReporter);

			resString += resIncomingNodesConstraint.first;
			resBool.append(resIncomingNodesConstraint.second);
		} else if (constraintType == "PropertyNode") {
			QPair<QString, QList<QString> > resPropertyNodeConstraint = countConstraintForPropertyNode(
					constraint
					, elementName
					, depth + additionalDepth
					, addStr);

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
				, addStr);

		resString += resMultiOrNodeConstraint.first;
		resBool.append(resMultiOrNodeConstraint.second);
	}

	mCountsOfConstraintElementsInOneConstraint[constraintType]++;

	QPair<QString, QList<QString> > resNeighborsNodes = countNeighborsElementsByOr(
			constraint
			, AuxiliaryMethods::conjunctionExpression(resBool)
			, usedElements
			, node, elementName
			, depth + additionalDepth
			, addStr);

	resString += resNeighborsNodes.first;

	if (!resNeighborsNodes.second.isEmpty()) {
		resBool = resNeighborsNodes.second;
	}

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));

	return QPair<QString, QList<QString> >(resString, allResultBool);
}
