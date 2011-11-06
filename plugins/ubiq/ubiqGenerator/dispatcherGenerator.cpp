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

QString DispatcherGenerator::generateEventHandler(QString const &handlerName)
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
			if (!mApi.isLogicalElement(element) || (element.element() != "HandlerStart"))
				continue;

			QString caseCode = mTemplateUtils["@@SwitchCase@@"];
			QString body = generateCaseBody(element);

			caseCode.replace("@@CaseValue@@", mApi.name(element))
					.replace("@@CaseCode@@", body);

			cases += caseCode;
		}
		handlerCode.replace("@@SwitchCases@@", cases);
	}

	return handlerCode;
}

QString DispatcherGenerator::generateCaseBody(qReal::Id const &handlerStart)
{
	QString body;

	IdList links = mApi.outgoingLinks(handlerStart);
	if (links.size() != 1) {
		mErrorReporter.addError(QObject::tr("HandlerStart node should have exactly 1 outgoing link"), handlerStart);
		return "";
	}

	Id nextNode = mApi.otherEntityFromLink(links.at(0), handlerStart);
	body += generateOperatorCode(nextNode);

	return body;
}

QString DispatcherGenerator::generateOperatorCode(qReal::Id const &currentNode)
{
	QString operatorCode = mTemplateUtils["@@CaseCode@@"];

	if (currentNode.element() == "FunctionCall") {
		QString code = mApi.name(currentNode) + "(";
		foreach (Id const &argument, mApi.children(currentNode)) {
			if (!mApi.isLogicalElement(argument) || (argument.element() != "ActualParameter"))
				continue;

			code += (mApi.name(argument) + ", ");
		}
		code.chop(2); // terminating space and comma
		code += ")";
		operatorCode.replace("@@Command@@", code);

		IdList links = mApi.outgoingLinks(currentNode);

		if (links.size() == 0)
			return operatorCode;
		else if (links.size() > 1) {
			mErrorReporter.addError(QObject::tr("FunctionCall node should have exactly 1 outgoing link"), currentNode);
			return "";
		}

		Id nextNode = mApi.otherEntityFromLink(links.at(0), currentNode);
		return operatorCode + generateOperatorCode(nextNode);
	}



	return "";
}

