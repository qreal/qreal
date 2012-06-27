#include "dataObjectGenerator.h"

QString const sbtDiagram = "SbtReportsEditor";
QString const dataObjectName = "DataObject";
QString const fileName = "DataObject.cs";

using namespace utils;

DataObjectGenerator::DataObjectGenerator(const QString &templateDirPath,
		const QString &outputDirPath,
		const LogicalModelAssistInterface &logicalModel,
		ErrorReporterInterface &errorReporter) :
		utils::AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
{
}

void DataObjectGenerator::generate()
{
	QString result;

	loadTemplateFromFile(fileName, result);
	loadUtilsTemplates();

  foreach (Id const &diagram, mApi.elementsByType(sbtDiagram)) {
	  if (!mApi.isLogicalElement(diagram)) {
		  continue;
	  }

	  foreach (Id const &element, mApi.children(diagram)) {
		  if (!mApi.isLogicalElement(element)) {
			  continue;
		  }

		  if (element.element() == dataObjectName) {
			  result.replace("@@DataObject@@", mApi.name(element))
					.replace("@@ListOfProperties@@", generatePropertiesCode(element));
		  }
		saveOutputFile(fileName + mApi.name(element), result);
	  }
  }
}
