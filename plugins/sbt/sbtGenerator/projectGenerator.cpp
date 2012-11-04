#include "projectGenerator.h"

QString const ProjecttName = "DataObject";
QString const fileName = "ProjectFile.csproj";

using namespace generatorsUtils;
using namespace qReal;
using namespace sbt;

ProjectGenerator::ProjectGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, LogicalModelAssistInterface const &logicalModel
		, ErrorReporterInterface &errorReporter)
		: AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
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
	foreach (QString const &fileName, files) {
		mFiles.append("<Compile Include=\"" + fileName + "\" />\n");
	}
}
