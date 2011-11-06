#include <QtCore/QFile>
#include <QtCore/QDebug>

#include "dispatcherGenerator.h"
#include "nameNormalizer.h"

using namespace ubiq::generator;
using namespace qReal;

DispatcherGenerator::DispatcherGenerator(QString const &templateDirPath
		, QString const &outputDirPath
		, qReal::LogicalModelAssistInterface const &logicalModel
		, qReal::ErrorReporterInterface &errorReporter
		)
		: AbstractGenerator(templateDirPath , outputDirPath, logicalModel, errorReporter)
{
}

DispatcherGenerator::~DispatcherGenerator()
{
}


void DispatcherGenerator::generate()
{
	loadUtilsTemplates();

	foreach (Id const diagram, mApi.elementsByType("MasterNode")) { // get master node
		if (!mApi.isLogicalElement(diagram))
			continue;

		QString const templateName = "DeviceDispatcher.cs";

		QString fileTemplate;
		loadTemplateFromFile(templateName, fileTemplate);

		QString eventHadlers = generateEventHandlers(diagram);


		fileTemplate.replace("@@EventHandlers@@", eventHadlers);

		saveOutputFile(templateName, fileTemplate);
	}
}

QString DispatcherGenerator::generateEventHandlers(Id const &diagram)
{
	QString eventHadlers;
	foreach (Id const element, mApi.children(diagram)) { // get elements on master node
		qDebug() << element.toString();
		if (!mApi.isLogicalElement(element) || (element.element() != "Handler"))
			continue;


		eventHadlers += generateEventHandler(mApi.name(element));
	}
	return eventHadlers;
}

QString DispatcherGenerator::generateEventHandler(QString const handlerName)
{
	return "88";
}

