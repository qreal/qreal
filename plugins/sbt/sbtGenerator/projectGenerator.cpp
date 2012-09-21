#include "projectGenerator.h"

QString const ProjecttName = "DataObject";
QString const fileName = "ProjectFile.csproj";

using namespace utils;

ProjectGenerator::ProjectGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, LogicalModelAssistInterface const &logicalModel
		, ErrorReporterInterface &errorReporter) :
		utils::AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
{
}

void ProjectGenerator::generate()
{
	QString result;

	loadTemplateFromFile(fileName, result);

	result.replace("@@Files@@", mFiles);
	saveOutputFile("GeneratedProjectFile.csproj", result);
}

void ProjectGenerator::setFilesName(QList<QString> const &files)
{
	foreach (QString fileName, files)
	{
	  mFiles.append("<Compile Include=\"" + fileName + "\" />\n");
	}
}
