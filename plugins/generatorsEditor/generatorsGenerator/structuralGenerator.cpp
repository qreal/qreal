#include "structuralGenerator.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::generator;
using namespace qReal;
using namespace utils;

/// Generation target file
QString const fileNamePRO = "generator.pro";
QString const fileNameGeneratorH = "generator.h";
QString const fileNameGeneratorCPP = "generator.cpp";
QString const fileNamePluginH = "generatorPlugin.h";
QString const fileNamePluginCPP = "generatorPlugin.cpp";

StructuralGenerator::StructuralGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, QString const &pathToQReal
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &metamodelLanguageName
		, QString const &languageName
		, QString const &nodeName
		, QString const &toGeneratePropertyName
		, QString const &programNamePropertyName
		, QString const &generatorMetamodelName
	)
	: MyAbstractGenerator(templateDirPath, outputDirPath, pathToQReal
						  , logicalModel, errorReporter
						  , metamodelLanguageName, languageName, nodeName
						  , toGeneratePropertyName, programNamePropertyName
						  , generatorMetamodelName)
{
}

StructuralGenerator::~StructuralGenerator()
{
}

QString StructuralGenerator::generateForeachNode(Id const &element, int count, int parentCount)
{
	QString result;
	int curCount = count * 10 + 1;
	foreach (Id const &child, mApi.children(element)) {
		if (!mApi.isLogicalElement(child) || child.type().element() != "SemanticNode") {
			continue;
		}
		result += generateSemanticNode(child, "oneForeachGenerateBody", curCount, parentCount) + "\n";
		curCount ++;
	}

	generateTemplateUsingTextCode(element, result);
	return result;
}

QString StructuralGenerator::generateConverterNode(Id const &element, int count)
{
	Q_UNUSED(element)
}

QStringList StructuralGenerator::marksList(QString const& text)
{
	QRegExp pattern = QRegExp("@@*@@");
	return text.split(pattern);
}

QString StructuralGenerator::generateSemanticNode(Id const &element, QString const& templateName, int count, int parentCount)
{
	QString elementName = mApi.property(element, "elementName").toString();
	QString textCode = mApi.property(element, "textCode").toString();

	QString genBody = mTemplateUtils["@@" + templateName + "@@"];

	if (!textCode.isEmpty()) {
		QString markName = "elements_" + elementName;
		mMarksCode[markName] = textCode;

		QString replaceTemplateBody = mTemplateUtils["@@oneReplaceTemplateGenerateBody@@"];
		QString replaceTemplates = "";
		foreach (QString var, marksList(textCode)) {
			replaceTemplates += mTemplateUtils["@@oneSimpleReplaceTemplate@@"]
					.replace("@@templateTextCode@@", var)
					.replace("@@templateMarkName@@", mMarksCode[var]);
		}
		genBody.replace("@@replaceTemplates@@", replaceTemplates);
		genBody.replace("@@replaceTemplateGenerateBody@@", replaceTemplateBody);
	}

	genBody.replace("@@count@@", QString("%1").arg(count));
	genBody.replace("@@elementName@@", elementName);
	if (parentCount != -1) {
		genBody.replace("@@parentCount@@", QString("%1").arg(parentCount));
	}

	QString foreachBody = "";
	QString converterBody = "";

	int curCount = count + 1;
	foreach (Id const &outElement, mApi.outgoingNodes(element)) {
		if (!mApi.isLogicalElement(outElement)) {
			continue;
		}
		if (outElement.type().element() == "ForeachNode") {
			QString oneForeachBody = generateForeachNode(outElement, curCount, count);
			foreachBody += oneForeachBody + "\n";
		} else if (outElement.type().element() == "ConverterNode") {
			QString oneConverterBody = generateConverterNode(outElement, curCount);
			converterBody += oneConverterBody  + "\n";
		} else {
			continue;
		}
		curCount ++;
	}

	genBody.replace("@@foreachGenerateBody@@", foreachBody);
	genBody.replace("@@converterGenerateBody@@", converterBody);

//	mElementSemanticGenerateBody[elementName] = genBody;
	return genBody;
}

QString StructuralGenerator::generateSemanticNodes()
{
	QString result;
	foreach (Id const &element, mApi.elementsByType("SemanticNode")) {
		if (!mApi.isLogicalElement(element) || mApi.parent(element).type().element() != "GeneratorDiagram") {
			continue;
		}
		result += generateSemanticNode(element, "oneSemanticGenerateBody", 0) + "\n";
	}
	return result;
}

void StructuralGenerator::generate()
{
	foreach (Id const &element, mApi.elementsByType("TemplateNode")) {
		if (!mApi.isLogicalElement(element)) {
			continue;
		}
		generateTemplate(element);
	}

	QString generateBody = generateSemanticNodes();

	saveTemplateUtils();

	QString resultPRO;
	QString resultPluginH;
	QString resultPluginCPP;
	QString resultGeneratorH;
	QString resultGeneratorCPP;

	QString fileBaseName = normalizerGeneratorModelName();
	QString generatorClassName = normalizerGeneratorClassModelName();
	QString pathToQReal = mPathToQReal;

	loadTemplateFromFile(fileNamePRO, resultPRO);
	loadTemplateFromFile(fileNamePluginH, resultPluginH);
	loadTemplateFromFile(fileNamePluginCPP, resultPluginCPP);
	loadTemplateFromFile(fileNameGeneratorH, resultGeneratorH);
	loadTemplateFromFile(fileNameGeneratorCPP, resultGeneratorCPP);

	resultPRO.replace("@@strangePathToQReal@@", pathToQReal.replace("/", "\\\\"));

	resultPRO.replace("@@pathToQReal@@", mPathToQReal);
	resultPluginH.replace("@@pathToQReal@@", mPathToQReal);
	resultPluginCPP.replace("@@pathToQReal@@", mPathToQReal);
	resultGeneratorH.replace("@@pathToQReal@@", mPathToQReal);
	resultGeneratorCPP.replace("@@pathToQReal@@", mPathToQReal);

	resultPRO.replace("@@generatorName@@", fileBaseName);
	resultPluginH.replace("@@generatorName@@", fileBaseName);
	resultPluginCPP.replace("@@generatorName@@", fileBaseName);
	resultGeneratorH.replace("@@generatorName@@", fileBaseName);
	resultGeneratorCPP.replace("@@generatorName@@", fileBaseName);

	resultPluginH.replace("@@generatorClassName@@", generatorClassName);
	resultPluginCPP.replace("@@generatorClassName@@", generatorClassName);

	resultGeneratorCPP.replace("@@templateConstStringNames@@", generateTemplateConstStringNames());
	resultGeneratorCPP.replace("@@generateBody@@", generateBody);

	resultPluginCPP.replace("@@languageNodeDiagram@@", mNodeName);
	resultPluginCPP.replace("@@programNameProperty@@", mProgramNamePropertyName);
	resultPluginCPP.replace("@@pathToGenerateProperty@@", mToGeneratePropertyName);

	saveOutputFile(QString(fileBaseName + ".pro"), resultPRO);
	saveOutputFile(QString(fileBaseName + "Plugin.h"), resultPluginH);
	saveOutputFile(QString(fileBaseName + "Plugin.cpp"), resultPluginCPP);
	saveOutputFile(QString(fileNameGeneratorH), resultGeneratorH);
	saveOutputFile(QString(fileNameGeneratorCPP), resultGeneratorCPP);
}

