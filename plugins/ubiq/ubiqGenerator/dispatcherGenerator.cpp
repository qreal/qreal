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
		if (!mApi.isLogicalElement(element) || (element.element() != "Handler"))
			continue;

		eventHadlers += generateEventHandler(mApi.name(element)); // generate code for each event handler
	}
	return eventHadlers;
}

QString DispatcherGenerator::generateConstants(qReal::Id const &element) const
{
	QString result;
	foreach (Id const id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "MasterDiagramConstant")
			continue;

		QString constantTemplate = mTemplateUtils["@@MasterDiagramConstant@@"];
		QString const name = mApi.name(id);
		QString const type = mApi.stringProperty(id, "type");
		QString const value = mApi.stringProperty(id, "value");
		constantTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Type@@", type)
				.replace("@@Value@@", value)
				;

		result += constantTemplate;
	}
	return result;
}

QString DispatcherGenerator::generateFields(qReal::Id const &element) const
{
	QString result;
	foreach (Id const id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "MasterDiagramField")
			continue;

		QString constantTemplate = mTemplateUtils["@@MasterDiagramField@@"];
		QString const name = mApi.name(id);
		QString const type = mApi.stringProperty(id, "type");
		QString const value = mApi.stringProperty(id, "value");
		constantTemplate.replace("@@Name@@", NameNormalizer::normalize(name))
				.replace("@@Type@@", type)
				.replace("@@Value@@", value)
				;

		result += constantTemplate;
	}
	return result;
}

QString DispatcherGenerator::generateEventHandler(QString const handlerName)
{
	QString handlerCode = mTemplateUtils["@@EventHandler@@"];
	handlerCode.replace("@@HandlerName@@", handlerName);

	foreach (Id const diagram, mApi.elementsByType("UbiqActivityDiagram")) {
		// search for the diagram with name handlerName
		if (!mApi.isLogicalElement(diagram) || (mApi.name(diagram) != handlerName))
			continue;

		// diagram found, now get HandlerStart-s and generate cases

		QString cases;
		foreach (Id const &element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(diagram) || (element.element() != "HandlerStart"))
				continue;

			QString caseCode = mTemplateUtils["@@SwitchCase@@"];
			caseCode.replace("@@CaseValue@@", mApi.name(element));

			cases += caseCode;
		}
		handlerCode.replace("@@SwitchCases@@", cases);
	}

	return handlerCode;
}
