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

		fileTemplate.replace("@@EventHandlers@@", eventHadlers)
				.replace("@@Constants@@", generateConstants(diagram))
				.replace("@@Fields@@", generateFields(diagram))
				.replace("@@MessageInputMethods@@", generateMessageInputMethods(diagram))
				;

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
		constantTemplate.replace("@@Name@@", name)
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

		QString const name = mApi.name(id);
		QString const type = mApi.stringProperty(id, "type");
		QString const value = mApi.stringProperty(id, "defaultValue");
		bool const isStatic = mApi.property(id, "static").toBool();

		QString fieldTemplate = value.isEmpty()
				? mTemplateUtils["@@MasterDiagramFieldWithoutInit@@"]
				: mTemplateUtils["@@MasterDiagramField@@"];

		fieldTemplate.replace("@@Name@@", name)
				.replace("@@Type@@", type)
				.replace("@@Value@@", value)
				.replace("@@OptionalStatic@@", isStatic ? "static " : "")
				;

		result += fieldTemplate;
	}
	return result;
}

QString DispatcherGenerator::generateMessageInputMethods(qReal::Id const &element) const
{
	QString result;
	foreach (Id const id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "Handler")
			continue;

		QString const name = mApi.name(id);
		QString messageInputClass = mTemplateUtils["@@" + name + "Class@@"].trimmed();
		QString const parameter = mApi.stringProperty(id, "MessageInputParameter");

		QString useMessageInputTemplate = mTemplateUtils["@@UseMessageInput@@"];

		useMessageInputTemplate.replace("@@MessageInputClass@@", messageInputClass)
				.replace("@@MessageInputParameter@@", parameter)
				;

		result += useMessageInputTemplate;
	}
	return result;
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
	IdList links = mApi.outgoingLinks(handlerStart);
	if (links.size() != 1) {
		mErrorReporter.addError(QObject::tr("HandlerStart node should have exactly 1 outgoing link"), handlerStart);
		return "";
	}

	Id const nextNode = mApi.otherEntityFromLink(links.at(0), handlerStart);
	return generateOperatorCode(nextNode);
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

