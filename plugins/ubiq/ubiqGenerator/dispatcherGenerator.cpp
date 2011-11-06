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

	foreach (Id const masterNode, mApi.elementsByType("MasterNode")) { // get master node
		if (!mApi.isLogicalElement(masterNode))
			continue;

		QString const templateName = "DeviceDispatcher.cs";

		QString fileTemplate;
		loadTemplateFromFile(templateName, fileTemplate);

		fileTemplate.replace("@@EventHandlers@@", generateEventHandlers(masterNode))
				.replace("@@Constants@@", generateConstants(masterNode))
				.replace("@@Fields@@", generateFields(masterNode))
				.replace("@@MessageInputMethods@@", generateMessageInputMethods(masterNode))
				.replace("@@HelperFunctions@@", generateHelperFunctions(masterNode))
				;

		saveOutputFile(templateName, fileTemplate);
	}
}

QString DispatcherGenerator::generateEventHandlers(Id const &diagram) const
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
		QString const messageInputClass = mTemplateUtils["@@" + name + "Class@@"].trimmed();
		QString const parameter = mApi.stringProperty(id, "MessageInputParameter");

		QString useMessageInputTemplate = mTemplateUtils["@@UseMessageInput@@"];

		useMessageInputTemplate.replace("@@MessageInputClass@@", messageInputClass)
				.replace("@@MessageInputParameter@@", parameter)
				;

		result += useMessageInputTemplate;
	}
	return result;
}

QString DispatcherGenerator::generateHelperFunctions(qReal::Id const &element) const
{
	QString result;
	foreach (Id const diagram, mApi.outgoingConnections(element)) {
		if (!mApi.isLogicalElement(diagram) || diagram.element() != "UbiqActivityDiagram")
			continue;

		foreach (Id const &id, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(id) || (id.element() != "FormalParameters"))
				continue;

			QString const name = mApi.name(id);
			QString returnType = mApi.stringProperty(id, "returnType");
			QString const parameters = generateFunctionParameters(id);
			QString body = generateFunctionBody(id);

			if (body.endsWith('\n'))
				body.chop(1);

			QString helperFunctionTemplate = mTemplateUtils["@@HelperFunction@@"];
			helperFunctionTemplate.replace("@@Name@@", name)
					.replace("@@Type@@", returnType)
					.replace("@@Params@@", parameters)
					.replace("@@Body@@", body)
					;

			result += helperFunctionTemplate;
		}
	}
	return result;
}

QString DispatcherGenerator::generateFunctionParameters(qReal::Id const &element) const
{
	QString result;
	foreach (Id const id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "FormalParameter")
			continue;

		QString argumentTemplate = mTemplateUtils["@@Argument@@"].trimmed();
		argumentTemplate.replace("@@ArgType@@", mApi.stringProperty(id, "type"))
				.replace("@@ArgName@@", mApi.name(id))
				;

		result += argumentTemplate + ", ";
	}
	result.chop(2);
	return result;
}

QString DispatcherGenerator::generateFunctionBody(qReal::Id const &element) const
{
	return generateCaseBody(element);
}

QString DispatcherGenerator::generateEventHandler(QString const &handlerName) const
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

			QString caseStmts;
			foreach (QString const &caseLabel, mApi.name(element).split(",", QString::SkipEmptyParts)) {
				QString caseStmtTemplate = mTemplateUtils["@@Case@@"];
				caseStmtTemplate.replace("@@CaseValue@@", caseLabel.trimmed());
				caseStmts += caseStmtTemplate;
			}

			QString caseCodeTemplate = mTemplateUtils["@@SwitchCase@@"];

			QString body = generateCaseBody(element);
			if (body.endsWith('\n'))
				body.chop(1);

			if (caseStmts.endsWith('\n'))
				caseStmts.chop(1);

			caseCodeTemplate.replace("@@Cases@@", caseStmts)
					.replace("@@CaseCode@@", body)
					;

			cases += caseCodeTemplate;
		}
		handlerCode.replace("@@SwitchCases@@", cases);
	}

	return handlerCode;
}

QString DispatcherGenerator::generateCaseBody(qReal::Id const &handlerStart) const
{
	IdList links = mApi.outgoingLinks(handlerStart);
	if (links.size() > 1) {
		mErrorReporter.addError(QObject::tr("Start node should have exactly 1 outgoing link"), handlerStart);
		return "";
	}

	if (links.size() == 0) {
		return "";
	}

	Id const nextNode = mApi.otherEntityFromLink(links.at(0), handlerStart);
	return generateOperatorCode(nextNode);
}

QString DispatcherGenerator::generateOperatorCode(qReal::Id const &currentNode) const
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

	if (currentNode.element() == "Action") {
		operatorCode = mApi.name(currentNode);

		IdList links = mApi.outgoingLinks(currentNode);

		if (links.size() == 0)
			return operatorCode;
		else if (links.size() > 1) {
			mErrorReporter.addError(QObject::tr("Action node should have exactly 1 outgoing link"), currentNode);
			return "";
		}

		Id nextNode = mApi.otherEntityFromLink(links.at(0), currentNode);
		return operatorCode + "\n" + generateOperatorCode(nextNode);
	}

	if (currentNode.element() == "ReturnAction") {
		QString const returnValue = mApi.name(currentNode).isEmpty()
				? ""
				: (" " + mApi.name(currentNode))
				;

		QString returnWithValueTemplate = mTemplateUtils["@@Return@@"];
		returnWithValueTemplate.replace("@@OptionalReturnValue@@", returnValue);

		operatorCode = returnWithValueTemplate;

		IdList links = mApi.outgoingLinks(currentNode);

		if (links.size() == 0)
			return operatorCode;
		else {
			mErrorReporter.addError(QObject::tr("ReturnAction node shall have no outgoing links"), currentNode);
			return "";
		}
	}

	return "";
}
