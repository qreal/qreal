#include "concreteGenerator.h"
#include "generatorParts/generatorForProperties.h"
#include "generatorParts/generatorForListsOfElements.h"
#include "generatorParts/generatorForDiagrams.h"

#include <qrutils/nameNormalizer.h>

#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace constraints::generator;
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
				, GeneratorForDiagrams::countRealConstraintOfDiagramElement(
				element
				, mCountsOfConstraintElementsInOneConstraint
				, mApi
				, mErrorReporter));
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
