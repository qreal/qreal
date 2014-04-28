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
		, QString const &toGeneratePropertyName
		, QString const &programNamePropertyName
		, QString const &generatorMetamodelName
	)
	: MyAbstractGenerator(templateDirPath, outputDirPath, pathToQReal
						  , logicalModel, errorReporter
						  , metamodelLanguageName, languageName
						  , toGeneratePropertyName, programNamePropertyName
						  , generatorMetamodelName)
{
}

StructuralGenerator::~StructuralGenerator()
{
}

void StructuralGenerator::generate()
{
	foreach (Id const &element, mApi.elementsByType("TemplateNode")) {
		if (!mApi.isLogicalElement(element)) {
			continue;
		}
		generateTemplate(element);
	}
	saveTemplateUtils();

	QString resultPRO;
	QString resultPluginH;
	QString resultPluginCPP;
	QString resultGeneratorH;
	QString resultGeneratorCPP;

	loadTemplateFromFile(fileNamePRO, resultPRO);
	loadTemplateFromFile(fileNamePluginH, resultPluginH);
	loadTemplateFromFile(fileNamePluginCPP, resultPluginCPP);
	loadTemplateFromFile(fileNameGeneratorH, resultGeneratorH);
	loadTemplateFromFile(fileNameGeneratorCPP, resultGeneratorCPP);

	QString fileBaseName = normalizerGeneratorModelName();
	saveOutputFile(QString(fileBaseName + ".pro"), resultPRO);
	saveOutputFile(QString(fileBaseName + "Plugin.h"), resultPluginH);
	saveOutputFile(QString(fileBaseName + "Plugin.cpp"), resultPluginCPP);
	saveOutputFile(QString(fileNameGeneratorH), resultGeneratorH);
	saveOutputFile(QString(fileNameGeneratorCPP), resultGeneratorCPP);
}

