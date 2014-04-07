#include "structuralGenerator.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::generator;
using namespace qReal;
using namespace utils;

/// Generation target file

StructuralGenerator::StructuralGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, QString const &pathToQReal
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &metamodelLanguageName
		, QString const &generatorMetamodelName
	)
	: MyAbstractGenerator(templateDirPath, outputDirPath, pathToQReal, logicalModel, errorReporter, metamodelLanguageName, generatorMetamodelName)
{
}

StructuralGenerator::~StructuralGenerator()
{
}

void StructuralGenerator::generate()
{
}
