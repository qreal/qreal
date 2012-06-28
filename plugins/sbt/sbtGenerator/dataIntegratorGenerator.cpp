#include "dataIntegratorGenerator.h"

QString const sbtDiagram = "SbtReportsDiagramNode";
QString const dataIntegratorName = "ORMDataIntegrator";
QString const fileName = "DataIntegrator";

using namespace utils;

DataIntegratorGenerator::DataIntegratorGenerator(const QString &templateDirPath,
		 const QString &outputDirPath,
		 const LogicalModelAssistInterface &logicalModel,
		 ErrorReporterInterface &errorReporter) :
	utils::AbstractGenerator(templateDirPath, outputDirPath, logicalModel, errorReporter)
{
}

void DataIntegratorGenerator::generate()
{
	QString result;

	loadTemplateFromFile(fileName + ".cs", result);
	//loadUtilsTemplates();

	foreach (Id const &diagram, mApi.elementsByType(sbtDiagram)) {
		if (!mApi.isLogicalElement(diagram)) {
			continue;
		}

		foreach (Id const &element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(element)) {
				continue;
			}
			if (element.element() == dataIntegratorName) {
				QString res = result;
				qReal::IdList outgoingLinks = mApi.outgoingLinks(element);
				if (outgoingLinks.empty()) {
				  qDebug() << "no outgoung links";
				}
				else
				{
					qReal::Id outgoingLink = outgoingLinks.at(0);
					qReal::Id returnDO = mApi.otherEntityFromLink(outgoingLink, element);
					res.replace("@@ReturnDataObject@@", mApi.name(returnDO));
				}
				qReal::IdList incomingLinks = mApi.incomingLinks(element);
				if (incomingLinks.empty()) {
					qDebug() << "no incoming links";
				}
				else
				{
				qReal::Id incomingLink = incomingLinks.at(0);
				qReal::Id parameterDO = mApi.otherEntityFromLink(incomingLink, element);
					res.replace("@@ParameterDataObject@@", mApi.name(parameterDO));
				}
				res.replace("@@DataIntegrator@@", mApi.name(element) + "DI")
					.replace("@@queryString@@", mApi.stringProperty(element, "RequestBody"));
				saveOutputFile(fileName + mApi.name(element) + ".cs", res);
			}
	   }
	}
}
