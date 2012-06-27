#include "dataObjectGenerator.h"

QString const sbtDiagram = "SbtReportsDiagramNode";
QString const dataObjectName = "DataObject";
QString const fileName = "DataObject";

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

	qDebug() << fileName;
	loadTemplateFromFile(fileName + ".cs", result);
	//loadUtilsTemplates();

  foreach (Id const &diagram, mApi.elementsByType(sbtDiagram)) {
	qDebug() << "diagram!!";
	if (!mApi.isLogicalElement(diagram)) {
		  continue;
	  }

	  foreach (Id const &element, mApi.children(diagram)) {
		  if (!mApi.isLogicalElement(element)) {
			  continue;
		  }
		  if (element.element() == dataObjectName) {
			QString res = result;
				res.replace("@@DataObject@@", mApi.name(element))
					.replace("@@ListOfProperties@@", generatePropertiesCode(element));
				saveOutputFile(fileName + mApi.name(element) + ".cs", res);
		  }
	  }
  }
}
