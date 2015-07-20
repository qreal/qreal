/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <QtCore/QFile>

#include "dispatcherGenerator.h"
#include <qrutils/nameNormalizer.h>

using namespace ubiq::generator;
using namespace qReal;
using namespace utils;

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

	foreach (Id const &masterNode, mApi.elementsByType("MasterNode")) {
		if (!mApi.isLogicalElement(masterNode)) {
			continue;
		}

		QString const templateName = "DeviceDispatcher.cs";

		QString fileTemplate;
		loadTemplateFromFile(templateName, fileTemplate);

		fileTemplate.replace("@@EventHandlers@@", generateEventHandlers(masterNode))
				.replace("@@InitCode@@", mApi.stringProperty(masterNode, "initCode"))
				.replace("@@OnTcpIpCloseHandler@@", mApi.stringProperty(masterNode, "onTcpIpCloseHandler"))
				.replace("@@Preprocessors@@", generatePreprocessors(masterNode))
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
	foreach (Id const &element, mApi.children(diagram)) {
		if (!mApi.isLogicalElement(element) || element.element() != "Handler") {
			continue;
		}

		eventHadlers += generateEventHandler(mApi.name(element));
	}
	return eventHadlers;
}

QString DispatcherGenerator::generatePreprocessors(Id const &masterNode) const
{
	QString preprocessors;
	foreach (Id const &element, mApi.children(masterNode)) {
		if (!mApi.isLogicalElement(element) || element.element() != "Preprocessor") {
			continue;
		}

		preprocessors += generatePreprocessor(mApi.name(element));
	}
	return preprocessors;
}

QString DispatcherGenerator::generateConstants(qReal::Id const &element) const
{
	QString result;
	foreach (Id const &id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "MasterDiagramConstant") {
			continue;
		}

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
	foreach (Id const &id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "MasterDiagramField") {
			continue;
		}

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
	foreach (Id const &id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "Handler") {
			continue;
		}

		QString const name = mApi.name(id);
		QString const messageInputMethod = mTemplateUtils["@@" + name + "Input@@"].trimmed();
		QString const parameter = mApi.stringProperty(id, "MessageInputParameter");

		QString useMessageInputTemplate = mTemplateUtils["@@UseMessageInput@@"];

		useMessageInputTemplate.replace("@@MessageInputMethod@@", messageInputMethod)
				.replace("@@MessageInputParameter@@", parameter)
				;

		result += useMessageInputTemplate;
	}
	return result;
}

QString DispatcherGenerator::generateHelperFunctions(qReal::Id const &element) const
{
	QString result;
	foreach (Id const &diagram, mApi.outgoingConnections(element)) {
		if (!mApi.isLogicalElement(diagram) || diagram.element() != "UbiqActivityDiagram") {
			continue;
		}

		foreach (Id const &id, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(id) || (id.element() != "FormalParameters")) {
				continue;
			}

			QString const name = mApi.name(id);
			QString const returnType = mApi.stringProperty(id, "returnType");
			QString const parameters = generateFunctionParameters(id);
			QString body = generateFunctionBody(id);

			if (body.endsWith('\n')) {
				body.chop(1);
			}

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
	foreach (Id const &id, mApi.children(element)) {
		if (!mApi.isLogicalElement(id) || id.element() != "FormalParameter") {
			continue;
		}

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

	foreach (Id const &diagram, mApi.elementsByType("UbiqActivityDiagram")) {
		// search for the diagram with name handlerName
		if (!mApi.isLogicalElement(diagram) || (mApi.name(diagram) != handlerName)) {
			continue;
		}

		// diagram found, now get HandlerStart-s and generate cases
		QString cases;
		foreach (Id const &element, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(element) || (element.element() != "HandlerStart")) {
				continue;
			}

			QString caseStmts;
			foreach (QString const &caseLabel, mApi.name(element).split(",", QString::SkipEmptyParts)) {
				QString caseStmtTemplate = mTemplateUtils["@@Case@@"];
				caseStmtTemplate.replace("@@CaseValue@@", caseLabel.trimmed());
				caseStmts += caseStmtTemplate;
			}

			QString caseCodeTemplate = mTemplateUtils["@@SwitchCase@@"];

			QString body = generateCaseBody(element);
			if (body.endsWith('\n')) {
				body.chop(1);
			}

			if (caseStmts.endsWith('\n')) {
				caseStmts.chop(1);
			}

			caseCodeTemplate.replace("@@Cases@@", caseStmts)
					.replace("@@CaseCode@@", body)
					;

			cases += caseCodeTemplate;
		}
		handlerCode.replace("@@SwitchCases@@", cases);
	}

	return handlerCode;
}

QString DispatcherGenerator::generatePreprocessor(QString const &preprocessorName) const
{
	QString preprocessorCode = mTemplateUtils["@@Preprocessor@@"];
	preprocessorCode.replace("@@PreprocessorName@@", preprocessorName);

	foreach (Id const &diagram, mApi.elementsByType("UbiqActivityDiagram")) {
		// search for the diagram with name preprocessorName
		if (!mApi.isLogicalElement(diagram) || (mApi.name(diagram) != preprocessorName)) {
			continue;
		}

		QString code;
		foreach (Id const &child, mApi.children(diagram)) {
			if (!mApi.isLogicalElement(child) || (child.element() != "InitialNode")) {
				continue;
			}

			code = generateOperatorCode(child).text;
		}

		preprocessorCode.replace("@@Code@@", code);
	}

	return preprocessorCode;
}

QString DispatcherGenerator::generateCaseBody(qReal::Id const &handlerStart) const
{
	IdList const links = mApi.outgoingLinks(handlerStart);
	if (links.size() > 1) {
		mErrorReporter.addError(QObject::tr("Start node should have exactly 1 outgoing link"), handlerStart);
		return "";
	}

	if (links.size() == 0) {
		return "";
	}

	Id const nextNode = mApi.otherEntityFromLink(links.at(0), handlerStart);
	return generateOperatorCode(nextNode).text;
}

DispatcherGenerator::CodeBranchGenerationResult DispatcherGenerator::generateFunctionCallCode(
		qReal::Id const &currentNode) const
{
	QString operatorCode = mTemplateUtils["@@CaseCode@@"];

	QString code = mApi.name(currentNode) + "(";
	foreach (Id const &argument, mApi.children(currentNode)) {
		if (!mApi.isLogicalElement(argument) || (argument.element() != "ActualParameter")) {
			continue;
		}

		code += (mApi.name(argument) + ", ");
	}
	code.chop(2); // terminating space and comma
	code += ")";

	QString returnValue;
	foreach (Id const &argument, mApi.children(currentNode)) {
		if (!mApi.isLogicalElement(argument) || (argument.element() != "ReturnValue")) {
			continue;
		}

		if (!returnValue.isEmpty()) {
			mErrorReporter.addError(QObject::tr("FunctionCall shall have no more than one return value"), currentNode);
			return CodeBranchGenerationResult("", currentNode);
		}

		QString returnValueTemplate = mTemplateUtils["@@ReturnValue@@"];
		QString const type = mApi.stringProperty(argument, "type").isEmpty()
				? ""
				: mApi.stringProperty(argument, "type") + " ";

		returnValueTemplate.replace("@@OptionalType@@", type)
				.replace("@@Name@@", mApi.name(argument))
				;

		returnValue += returnValueTemplate;
	}

	if (returnValue.endsWith('\n')) {
		returnValue.chop(1);
	}

	code = returnValue + code;

	operatorCode.replace("@@Command@@", code);

	IdList const links = mApi.outgoingLinks(currentNode);

	if (links.size() == 0) {
		return CodeBranchGenerationResult(operatorCode, currentNode);
	} else if (links.size() > 1) {
		mErrorReporter.addError(QObject::tr("FunctionCall node should have exactly 1 outgoing link"), currentNode);
		return CodeBranchGenerationResult("", currentNode);
	}

	Id const nextNode = mApi.otherEntityFromLink(links.at(0), currentNode);

	CodeBranchGenerationResult result = generateOperatorCode(nextNode);
	result.text = operatorCode + result.text;
	return result;
}

DispatcherGenerator::CodeBranchGenerationResult DispatcherGenerator::generateActionCode(
		qReal::Id const &currentNode) const
{
	QString operatorCode = mApi.name(currentNode);

	IdList const links = mApi.outgoingLinks(currentNode);

	if (links.size() == 0) {
		return CodeBranchGenerationResult(operatorCode, currentNode);
	} else if (links.size() > 1) {
		mErrorReporter.addError(QObject::tr("Action node should have exactly 1 outgoing link"), currentNode);
		return CodeBranchGenerationResult("", currentNode);
	}

	Id const nextNode = mApi.otherEntityFromLink(links.at(0), currentNode);

	CodeBranchGenerationResult result = generateOperatorCode(nextNode);
	result.text = operatorCode + "\n" + result.text;
	return result;
}

DispatcherGenerator::CodeBranchGenerationResult DispatcherGenerator::generateInitialNodeCode(
		qReal::Id const &currentNode) const
{
	IdList const links = mApi.outgoingLinks(currentNode);

	if (links.size() != 1) {
		mErrorReporter.addError(QObject::tr("Initial node should have exactly 1 outgoing link"), currentNode);
		return CodeBranchGenerationResult("", currentNode);
	}

	Id const nextNode = mApi.otherEntityFromLink(links.at(0), currentNode);

	return generateOperatorCode(nextNode);
}

DispatcherGenerator::CodeBranchGenerationResult DispatcherGenerator::generateReturnActionCode(
		qReal::Id const &currentNode) const
{
	QString const returnValue = mApi.name(currentNode).isEmpty()
			? ""
			: (" " + mApi.name(currentNode))
			;

	QString returnWithValueTemplate = mTemplateUtils["@@Return@@"];
	returnWithValueTemplate.replace("@@OptionalReturnValue@@", returnValue);

	IdList const links = mApi.outgoingLinks(currentNode);

	if (links.size() == 0) {
		return CodeBranchGenerationResult(returnWithValueTemplate, currentNode);
	} else {
		mErrorReporter.addError(QObject::tr("ReturnAction node shall have no outgoing links"), currentNode);
		return CodeBranchGenerationResult("", currentNode);
	}
}

DispatcherGenerator::CodeBranchGenerationResult DispatcherGenerator::generateDecisionNodeCode(
		qReal::Id const &currentNode) const
{
	Id thenBranch;
	Id elseBranch;
	Id *currentBranch = nullptr;

	foreach (Id const &link, mApi.outgoingLinks(currentNode)) {
		if (!mApi.stringProperty(link, "guard").isEmpty()) {
			currentBranch = &thenBranch;
		} else {
			currentBranch = &elseBranch;
		}
		if (*currentBranch == Id()) {
			*currentBranch = link;
		} else {
			mErrorReporter.addError(QObject::tr("Decision node shall have exactly one outgoing link without guard")
					, currentNode);

			return CodeBranchGenerationResult("", currentNode);
		}
	}

	if (thenBranch == Id()) {
		mErrorReporter.addError(QObject::tr("Decision node shall have exactly one outgoing link with guard")
				, currentNode);

		return CodeBranchGenerationResult("", currentNode);
	}
	if (elseBranch == Id()) {
		mErrorReporter.addError(QObject::tr("Decision node shall have exactly one outgoing link without guard")
				, currentNode);

		return CodeBranchGenerationResult("", currentNode);
	}

	QString const guard = mApi.stringProperty(thenBranch, "guard");

	QString ifTemplate = mTemplateUtils["@@If@@"];

	CodeBranchGenerationResult thenBranchGenerationResult = generateOperatorCode(mApi.otherEntityFromLink(thenBranch
			, currentNode));

	CodeBranchGenerationResult elseBranchGenerationResult = generateOperatorCode(mApi.otherEntityFromLink(elseBranch
			, currentNode));

	if (thenBranchGenerationResult.text.endsWith('\n')) {
		thenBranchGenerationResult.text.chop(1);
	}
	if (elseBranchGenerationResult.text.endsWith('\n')) {
		elseBranchGenerationResult.text.chop(1);
	}

	Id nextNode;
	if (thenBranchGenerationResult.stopNode.element() == "MergeNode") {
		nextNode = thenBranchGenerationResult.stopNode;
	}
	if (elseBranchGenerationResult.stopNode.element() == "MergeNode") {
		if (elseBranchGenerationResult.stopNode != nextNode) {
			mErrorReporter.addError(QObject::tr("Decision branches shall end on the same merge node"), currentNode);
			return CodeBranchGenerationResult("", currentNode);
		}
	}

	if (elseBranchGenerationResult.text.trimmed().isEmpty()) {
		ifTemplate = mTemplateUtils["@@IfWithoutElse@@"];
	}

	ifTemplate.replace("@@Condition@@", guard)
			.replace("@@Then@@", thenBranchGenerationResult.text)
			.replace("@@Else@@", elseBranchGenerationResult.text)
			;

	if (nextNode == Id()) {
		return CodeBranchGenerationResult(ifTemplate, nextNode);
	}

	IdList const links = mApi.outgoingLinks(nextNode);
	if (links.size() == 0) {
		return CodeBranchGenerationResult(ifTemplate, nextNode);
	} else if (links.size() > 1) {
		mErrorReporter.addError(QObject::tr("Branches merge node should have no more than 1 outgoing link")
				, currentNode);

		return CodeBranchGenerationResult("", nextNode);
	}

	Id const nextNodeToGenerate = mApi.otherEntityFromLink(links.at(0), nextNode);

	CodeBranchGenerationResult result = generateOperatorCode(nextNodeToGenerate);
	result.text = ifTemplate + "\n" + result.text;
	return result;
}

DispatcherGenerator::CodeBranchGenerationResult DispatcherGenerator::generateOperatorCode(
		qReal::Id const &currentNode) const
{
	if (currentNode.element() == "FunctionCall") {
		return generateFunctionCallCode(currentNode);
	}

	if (currentNode.element() == "Action") {
		return generateActionCode(currentNode);
	}

	if (currentNode.element() == "InitialNode") {
		return generateInitialNodeCode(currentNode);
	}

	if (currentNode.element() == "ReturnAction") {
		return generateReturnActionCode(currentNode);
	}

	if (currentNode.element() == "DecisionNode") {
		generateDecisionNodeCode(currentNode);
	}

	return CodeBranchGenerationResult("", currentNode);
}
