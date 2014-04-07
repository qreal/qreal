#include "myAbstractGenerator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::generator;
using namespace qReal;
using namespace utils;

/// Generation target file

MyAbstractGenerator::MyAbstractGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, QString const &pathToQReal
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &metamodelLanguageName
		, QString const &generatorMetamodelName
		)
	: AbstractGenerator(templateDirPath, outputDirPath + QString("/generator" + generatorMetamodelName + "/"), logicalModel, errorReporter)
	, mPathToQReal("../" + pathToQReal), mMetamodelName(metamodelLanguageName), mGeneratorName(generatorMetamodelName)
{
	mPathToQReal.replace("\\", "/");
}

MyAbstractGenerator::~MyAbstractGenerator()
{
}

QString MyAbstractGenerator::generatorModelFullName()
{
	return mOutputDirPath;// + "generator" + mMetamodelName + ".pro";
}

QString MyAbstractGenerator::generatorModelName()
{
	return "generator" + mMetamodelName;
}

QString MyAbstractGenerator::generatorGeneratorModelName() //i.e. pliginName
{
	return "generator" + mGeneratorName;
}

QString MyAbstractGenerator::generatorNormalizerModelName()
{
	return NameNormalizer::normalize(generatorModelName(), false);
}

QString MyAbstractGenerator::generatorNormalizerGeneratorModelName() //i.e. normalizerPluginName
{
	return NameNormalizer::normalize(generatorGeneratorModelName(), false);
}

QString MyAbstractGenerator::generatorModelId() //i.e. pliginId
{
	return mGeneratorName;
}
