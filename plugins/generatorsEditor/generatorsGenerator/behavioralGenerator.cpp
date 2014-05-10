#include "behavioralGenerator.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::generator;
using namespace qReal;

/// Generation target file

BehavioralGenerator::BehavioralGenerator(QString const &templateDirPath
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

BehavioralGenerator::~BehavioralGenerator()
{
}

void BehavioralGenerator::generate()
{
}

