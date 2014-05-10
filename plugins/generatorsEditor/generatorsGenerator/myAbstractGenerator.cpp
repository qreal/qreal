#include "myAbstractGenerator.h"
#include "../../../qrutils/nameNormalizer.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace generators::generator;
using namespace qReal;
using namespace utils;

/// Generation target file
QString const generatedUtilsFileName = "utils.template";

MyAbstractGenerator::MyAbstractGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, QString const &pathToQReal
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		, QString const &metamodelName
		, QString const &languageName
		, QString const &nodeName
		, QString const &toGeneratePropertyName
		, QString const &programNamePropertyName
		, QString const &generatorMetamodelName
		)
	: AbstractGenerator(templateDirPath, outputDirPath + QString("/" + NameNormalizer::normalize(generatorMetamodelName, false) + "/"), logicalModel, errorReporter)
	, mPathToQReal(pathToQReal)
	, mMetamodelName(metamodelName)
	, mLanguageName(languageName)
	, mNodeName(nodeName)
	, mToGeneratePropertyName(toGeneratePropertyName)
	, mProgramNamePropertyName(programNamePropertyName)
	, mGeneratorName(generatorMetamodelName)
	, mTemplateDirName("/templates")
{
	mPathToQReal.replace("\\", "/");
}

MyAbstractGenerator::~MyAbstractGenerator()
{
}

QString MyAbstractGenerator::modelFullName()
{
	return mOutputDirPath;
}

QString MyAbstractGenerator::metamodelName()
{
	return mMetamodelName;
}

QString MyAbstractGenerator::languageName()
{
	return mLanguageName;
}

QString MyAbstractGenerator::generatorModelName() //i.e. pliginName
{
	return mGeneratorName;
}

QString MyAbstractGenerator::normalizerMetamodelName()
{
	return NameNormalizer::normalize(metamodelName(), false);
}

QString MyAbstractGenerator::normalizerLanguageName()
{
	return NameNormalizer::normalize(languageName(), false);
}

QString MyAbstractGenerator::normalizerGeneratorModelName() //i.e. normalizerPluginName
{
	return NameNormalizer::normalize(generatorModelName(), false);
}

QString MyAbstractGenerator::normalizerGeneratorClassModelName()
{
	return NameNormalizer::normalize(generatorModelName(), true);
}

void MyAbstractGenerator::generateTemplate(Id const &element)
{
	QString textCode = mApi.property(element, "textCode").toString();
	generateTemplateUsingTextCode(element, textCode);
}

void MyAbstractGenerator::generateTemplateUsingTextCode(Id const &element, QString const& textCode)
{
	QString markName = mApi.property(element, "markName").toString();
	QString fileName = mApi.property(element, "fileName").toString();

	if (!fileName.isEmpty()) {
		saveOutputFile(QString(fileName), textCode, mTemplateDirName);
		QString variableName = QString(fileName).replace(".", "_");
		mTemplateVariableFilename[variableName] = fileName;
	} else if (!markName.isEmpty()) {
		mMarksCode[markName] = textCode;
	} else {
		mErrorReporter.addCritical(QObject::tr("FileName or MarkName of \'%1\' not found").arg(element.element())
								   , element);
	}
}

void MyAbstractGenerator::saveTemplateUtils()
{
	QString result = "";
	foreach (QString mark, mMarksCode.keys()) {
		result += "@@" + mark + "@@\n" + mMarksCode[mark] + "\n==========\n";
	}
	saveOutputFile(QString(generatedUtilsFileName), result, mTemplateDirName);
}

QString MyAbstractGenerator::generateTemplateConstStringNames()
{
	QString result;
	foreach (QString var, mTemplateVariableFilename.keys()) {
		QString templateConstStringName  = mTemplateUtils["@@templateConstStringName@@"];
		templateConstStringName.replace("@@templateVariableName@@", var);
		templateConstStringName.replace("@@templateFileName@@", mTemplateVariableFilename[var]);

		result += templateConstStringName;
	}
	return result;
}
