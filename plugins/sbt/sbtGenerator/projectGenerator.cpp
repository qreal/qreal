#include "projectGenerator.h"

QString const ProjecttName = "DataObject";
QString const fileName = "ProjectFile.csproj";

using namespace utils;

ProjectGenerator::ProjectGenerator(const QString &templateDirPath,
		const QString &outputDirPath,
		const LogicalModelAssistInterface &logicalModel,
		ErrorReporterInterface &errorReporter) :
		utils::AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
{
}

void ProjectGenerator::generate()
{
	QString result;

	loadTemplateFromFile(fileName, result);
	//loadUtilsTemplates();

	result.replace("@@Files@@", mFiles);
	saveOutputFile("GeneratedProjectFile.csproj", result);
}

void ProjectGenerator::setFilesName(const QList<QString> &files)
{
	foreach (QString fileName, files)
	{
	  mFiles.append("<Compile Include=\"" + fileName + "\" />\n");
	}
}
