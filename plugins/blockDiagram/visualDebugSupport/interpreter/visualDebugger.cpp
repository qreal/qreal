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

#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QFile>

#include "visualDebugger.h"

#include <qrgui/models/propertyEditorModel.h>
#include <qrgui/models/models.h>

#include <qrkernel/exception/exception.h>

using namespace qReal;

VisualDebugger::VisualDebugger(qReal::LogicalModelAssistInterface const &logicalModelApi
		, qReal::GraphicalModelAssistInterface const &graphicalModelApi
		, qReal::gui::MainWindowInterpretersInterface &interpretersInterface
		, qReal::BlockParser *blockParser)
		: mCurrentDiagram(Id::rootId())
		, mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mError(VisualDebugger::noErrors)
		, mCurrentId(Id::rootId())
		, mBlockParser(blockParser)
		, mTimeout(750)
		, mDebugType(VisualDebugger::noDebug)
		, mHasCodeGenerationError(false)
		, mHasNotEndWithFinalNode(false)
		, mCodeFileName("code.c")
		, mWorkDir("")
{
	if (!blockParser) {
		mBlockParser = new BlockParser(interpretersInterface.errorReporter());
	}
}

VisualDebugger::~VisualDebugger()
{
	delete mBlockParser;
}

QVariant VisualDebugger::getProperty(Id const &id, QString const &propertyName)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	}

	return mLogicalModelApi.logicalRepoApi().property(mGraphicalModelApi.logicalId(id), propertyName);
}

void VisualDebugger::setDebugType(VisualDebugger::DebugType type)
{
	mDebugType = type;
}

void VisualDebugger::setTimeout(int timeout)
{
	mTimeout = timeout;
}

void VisualDebugger::setDebugColor(QString const &color)
{
	mDebugColor = QColor(color);
}

void VisualDebugger::setCurrentDiagram()
{
	Id const currentDiagram = mInterpretersInterface.activeDiagram();
	if (currentDiagram == mCurrentDiagram || Id::rootId() == mCurrentDiagram || Id::rootId() == mCurrentId) {
		mCurrentDiagram = currentDiagram;
	} else {
		mError = VisualDebugger::someDiagramIsRunning;
	}
}

bool VisualDebugger::canDebug(DebugType type)
{
	bool res = false;
	switch (type) {
		case VisualDebugger::singleStepDebug:
			res = mDebugType != VisualDebugger::fullDebug && mDebugType != VisualDebugger::debugWithDebugger;
			break;
		case VisualDebugger::fullDebug:
			res = mDebugType == VisualDebugger::noDebug;
			break;
		case VisualDebugger::debugWithDebugger:
			res = mDebugType != VisualDebugger::fullDebug && mDebugType != VisualDebugger::singleStepDebug;
			break;
		default:
			res = false;
			break;
	}
	res = res && mError == VisualDebugger::noErrors;

	if (!res) {
		error(VisualDebugger::someDiagramIsRunning);
	}
	return res;
}

void VisualDebugger::error(ErrorType e)
{
	switch (e) {
		case missingBeginNode:
			mInterpretersInterface.errorReporter()->addCritical(tr("The diagram doesn't have Initial Node"));
			break;
		case missingEndNode:
			mInterpretersInterface.errorReporter()->addCritical(tr("The diagram doesn't end with Final Node"));
			break;
		case missingEndOfLinkNode:
			mInterpretersInterface.errorReporter()->addCritical(tr("The diagram cann't end with edge"), mCurrentId);
			break;
		case endWithNotEndNode:
			mInterpretersInterface.errorReporter()->addWarning(
					tr("There are no links from this node and it mismatches Final Node")
					, mCurrentId);
			break;
		case missingValidLink:
			mInterpretersInterface.errorReporter()->addCritical(tr("The condition doesn't have valid link")
					, mCurrentId);
			break;
		case someDiagramIsRunning:
			mInterpretersInterface.errorReporter()->addCritical(tr("Some diagram is already under debug"));
			mError = VisualDebugger::noErrors;
			return;
			break;
		case codeGenerationError:
			mInterpretersInterface.errorReporter()->addCritical(tr("Code generation failed"));
			mHasCodeGenerationError = true;
			break;
		case incorrectUseOfLink:
			mInterpretersInterface.errorReporter()->addCritical(tr("Incorrect type of link used"), mCurrentId);
			break;
		case noErrors:
			return;
			break;
	}
	deinitialize();
}

Id const VisualDebugger::findBeginNode(QString const &name)
{
	IdList const children = mGraphicalModelApi.graphicalRepoApi().children(mCurrentDiagram);
	foreach (Id const &child, children) {
		if (child.element() == name) {
			return child;
		}
	}

	error(VisualDebugger::missingBeginNode);
	return Id::rootId();
}

Id const VisualDebugger::findValidLink()
{
	IdList const outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);
	QString const conditionStr = getProperty(mCurrentId, "condition").toString();
	int pos = 0;
	bool condition = mBlockParser->parseCondition(conditionStr, pos, mCurrentId);

	foreach (Id const &link, outLinks) {
		if (checkForIncorrectUseOfLink(link, "ControlFlow")) {
			return Id::rootId();
		}

		bool type = getProperty(link, "type").toBool();
		if (type == condition) {
			return link;
		}
	}

	if (!mBlockParser->hasErrors()) {
		error(VisualDebugger::missingValidLink);
	}

	return Id::rootId();
}

void VisualDebugger::pause(int time)
{
	QEventLoop loop;
	QTimer::singleShot(time, &loop, SLOT(quit()));
	loop.exec();
}

bool VisualDebugger::isFinalNode(Id const &id)
{
	IdList const outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
	return outLinks.count() == 0 && id.element() == "BlockFinalNode";
}

bool VisualDebugger::hasEndOfLinkNode(Id const &id)
{
	return mLogicalModelApi.logicalRepoApi().to(id) != Id::rootId();
}

VisualDebugger::ErrorType VisualDebugger::doFirstStep(Id const &id)
{
	if (id == Id::rootId()) {
		return VisualDebugger::missingBeginNode;
	}

	mCurrentId = id;
	highlight(mCurrentId);

	return VisualDebugger::noErrors;
}

void VisualDebugger::doStep(Id const &id)
{
	mCurrentId = id;
	highlight(mCurrentId);

	if (mCurrentId.element() == "Action") {
		processAction();
	}
}

void VisualDebugger::deinitialize()
{
	dehighlight();
	mCurrentId = Id::rootId();
	mCurrentDiagram = Id::rootId();
	mError = VisualDebugger::noErrors;
	mBlockParser->clear();
	mDebugType = VisualDebugger::noDebug;
}

void VisualDebugger::processAction()
{
	int pos = 0;
	mBlockParser->parseProcess(getProperty(mCurrentId, "process").toString(), pos, mCurrentId);
}

void VisualDebugger::debug()
{
	mDebugType = VisualDebugger::fullDebug;
	setTimeout(SettingsManager::value("debuggerTimeout").toInt());
	setDebugColor(SettingsManager::value("debugColor").toString());


	if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
		return;
	}

	mBlockParser->setErrorReporter(mInterpretersInterface.errorReporter());

	IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);

	while (outLinks.count() > 0) {
		pause(mTimeout);

		if (mCurrentId.element() == "ConditionNode") {
			Id const validLinkId = findValidLink();
			if (mBlockParser->hasErrors()) {
				deinitialize();
				return;
			}

			if (validLinkId != Id::rootId()) {
				doStep(validLinkId);
			} else {
				return;
			}
		} else {
			if (checkForIncorrectUseOfLink(outLinks.at(0), "ConditionControlFlow")) {
				return;
			}

			doStep(outLinks.at(0));
			if (mBlockParser->hasErrors()) {
				deinitialize();
				return;
			}
		}

		pause(mTimeout);

		if (!hasEndOfLinkNode(mCurrentId)) {
			error(VisualDebugger::missingEndOfLinkNode);
			return;
		}

		doStep(mLogicalModelApi.logicalRepoApi().to(mCurrentId));
		if (mBlockParser->hasErrors()) {
			deinitialize();
			return;
		}

		outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);
	}

	pause(mTimeout);

	if (!isFinalNode(mCurrentId)) {
		error(VisualDebugger::endWithNotEndNode);
		return;
	}

	mInterpretersInterface.errorReporter()->addInformation(tr("Debug finished successfully"));
	deinitialize();
	return;
}

void VisualDebugger::debugSingleStep()
{
	mDebugType = VisualDebugger::singleStepDebug;
	setDebugColor(SettingsManager::value("DebugColor").toString());

	if (mCurrentId == Id::rootId()) {
		if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
			return;
		}
	} else {
		mBlockParser->setErrorReporter(mInterpretersInterface.errorReporter());

		if (mCurrentId.element() == "ControlFlow" || mCurrentId.element() == "ConditionControlFlow")
		{
			if (!hasEndOfLinkNode(mCurrentId)) {
				error(VisualDebugger::missingEndOfLinkNode);
				return;
			}

			doStep(mLogicalModelApi.logicalRepoApi().to(mCurrentId));

			if (mBlockParser->hasErrors()) {
				deinitialize();
				return;
			}
		} else if (mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId).count() == 0) {
			if (!isFinalNode(mCurrentId)) {
				error(VisualDebugger::endWithNotEndNode);
				return;
			}

			deinitialize();
			mInterpretersInterface.errorReporter()->addInformation(tr("Debug (single step) finished successfully"));

			return ;
		} else if (mCurrentId.element() == "ConditionNode") {
			Id const validLinkId = findValidLink();

			if (mBlockParser->hasErrors()) {
				deinitialize();
				return;
			}

			if (validLinkId != Id::rootId()) {
				doStep(validLinkId);
			} else {
				return;
			}
		} else {
			IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);
			if (checkForIncorrectUseOfLink(outLinks.at(0), "ConditionControlFlow")) {
				return;
			}

			doStep(outLinks.at(0));

			if (mBlockParser->hasErrors()) {
				deinitialize();
				return;
			}
		}

		mInterpretersInterface.errorReporter()->addInformation(tr("Debug (single step) finished successfully"));
		return;
	}

	mInterpretersInterface.errorReporter()->addInformation(tr("Debug (single step) finished successfully"));
	return;
}

void VisualDebugger::generateCode()
{
	mHasCodeGenerationError = false;

	setCodeFileName(SettingsManager::value("codeFileName").toString());
	setWorkDir(SettingsManager::value("debugWorkingDirectory").toString());

	QFile codeFile(mWorkDir + mCodeFileName);
	codeFile.open(QIODevice::WriteOnly);

	codeFile.write("void main(int argc, char* argv[]) {\n");
	Id const startId = findBeginNode("InitialNode");

	if (startId != Id::rootId()) {
		generateCode(startId, codeFile);
		codeFile.write("}");
		codeFile.close();
		return;
	}

	codeFile.close();
	error(codeGenerationError);
	return;
}

void VisualDebugger::getConditionLinks(IdList const &outLinks, Id &falseEdge, Id &trueEdge)
{
	foreach (Id const &outLink, outLinks) {
		if (checkForIncorrectUseOfLink(outLink, "ControlFlow")) {
			error(codeGenerationError);
			return;
		}

		bool type = getProperty(outLink, "type").toBool();
		if (type) {
			trueEdge = outLink;
		} else {
			falseEdge = outLink;
		}
	}
}

void VisualDebugger::generateCode(Id const &id, QFile &codeFile)
{
	if (id.element() == "Action") {
		QString const code = getProperty(id, "process").toString();
		if (code.mid(0,4) == "var ") {
			codeFile.write(code.mid(4).toLatin1());
		} else {
			codeFile.write(code.toLatin1());
		}

		codeFile.write("\n");
		if (mLogicalModelApi.logicalRepoApi().outgoingLinks(id).count() != 0) {
			Id const nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(id).at(0);
			if (checkForIncorrectUseOfLink(nextEdge, "ConditionControlFlow")) {
				error(codeGenerationError);
				return;
			}

			generateCode(nextEdge, codeFile);
		} else {
			error(VisualDebugger::missingEndNode);
			error(codeGenerationError);
			return;
		}
	} else if (id.element() == "ConditionNode") {
			codeFile.write("if (");
			codeFile.write(getProperty(id, "condition").toByteArray());
			codeFile.write(") {\n");

			IdList const outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
			Id falseEdge = falseEdge.rootId();
			Id trueEdge = trueEdge.rootId();

			getConditionLinks(outLinks, falseEdge, trueEdge);

			if (mHasCodeGenerationError) {
				return;
			}

			if (trueEdge == trueEdge.rootId()) {
				error(VisualDebugger::missingValidLink);
				error(codeGenerationError);
				return;
			}

			generateCode(trueEdge, codeFile);
			codeFile.write("}\n");

			if (falseEdge != falseEdge.rootId()) {
				codeFile.write("else {\n");
				generateCode(falseEdge, codeFile);
				codeFile.write("}\n");
			}
	} else if (id.element() == "InitialNode") {
		Id const nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(id).at(0);
		if (checkForIncorrectUseOfLink(nextEdge, "ConditionControlFlow")) {
			error(codeGenerationError);
			return;
		}
		generateCode(nextEdge, codeFile);
	} else if (id.element() == "BlockFinalNode") {
		return;
	} else {
		Id const nextNode = mLogicalModelApi.logicalRepoApi().to(id);
		generateCode(nextNode, codeFile);
	}
}

void VisualDebugger::createIdByLineCorrelation()
{
	mHasNotEndWithFinalNode = false;
	int line = 2;
	Id const startId = findBeginNode("InitialNode");
	mIdByLineCorrelation[1] = startId;
	createIdByLineCorrelation(startId, line);
}

void VisualDebugger::createIdByLineCorrelation(Id const &id, int& line)
{
	if (id.element() == "Action") {
		mIdByLineCorrelation[line] = id;
		line++;

		if (mLogicalModelApi.logicalRepoApi().outgoingLinks(id).count() != 0) {
			Id const nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(id).at(0);
			if (checkForIncorrectUseOfLink(nextEdge, "ConditionControlFlow")) {
				error(codeGenerationError);
				return;
			}
			createIdByLineCorrelation(nextEdge, line);
		} else {
			error(missingEndNode);
			mHasNotEndWithFinalNode = true;
		}
	} else if (id.element() == "ConditionNode") {
		mIdByLineCorrelation[line] = id;
		line++;
		IdList const outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
		Id falseEdge = falseEdge.rootId();
		Id trueEdge = trueEdge.rootId();

		getConditionLinks(outLinks, falseEdge, trueEdge);

		if (mHasCodeGenerationError) {
			return;
		}

		createIdByLineCorrelation(trueEdge, line);
		line++;

		if (falseEdge != falseEdge.rootId()) {
			line++;
			createIdByLineCorrelation(falseEdge, line);
			line++;
		}
	} else if (id.element() == "InitialNode") {
		Id const nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(id).at(0);
		if (checkForIncorrectUseOfLink(nextEdge, "ConditionControlFlow")) {
			error(codeGenerationError);
			return;
		}
		createIdByLineCorrelation(nextEdge, line);
	} else if (id.element() == "BlockFinalNode") {
		mIdByLineCorrelation[line] = id;
	} else {
		Id const nextNode  = mLogicalModelApi.logicalRepoApi().to(id);
		createIdByLineCorrelation(nextNode, line);
	}
}

QList<int>* VisualDebugger::computeBreakpoints()
{
	QList<int> *breakpoints = new QList<int>();
	int line = 1;
	Id curId = mIdByLineCorrelation[line];

	while (mIdByLineCorrelation[line].element() != "BlockFinalNode") {
		while (mIdByLineCorrelation.contains(line) && curId.toString() == mIdByLineCorrelation[line].toString()) {
			line++;
		}

		breakpoints->append(line - 1);

		while (!mIdByLineCorrelation.contains(line)) {
			line++;
		}

		curId = mIdByLineCorrelation[line];
	}
	breakpoints->append(line);
	return breakpoints;
}

Id VisualDebugger::getIdByLine(int line)
{
	return mIdByLineCorrelation[line];
}

void VisualDebugger::highlight(Id const &id)
{
	mInterpretersInterface.highlight(id, true);
}

void VisualDebugger::dehighlight()
{
	mInterpretersInterface.dehighlight();
}

bool VisualDebugger::canBuild()
{
	return !mHasCodeGenerationError;
}

void VisualDebugger::setCodeFileName(QString const &name)
{
	mCodeFileName = name;
}

bool VisualDebugger::canComputeBreakpoints()
{
	return !mHasNotEndWithFinalNode;
}

void VisualDebugger::setWorkDir(QString const &path)
{
	if (path != "") {
		mWorkDir = path + "/";
	}
}

bool VisualDebugger::checkForIncorrectUseOfLink(Id const &link, QString const &type)
{
	bool res = link.element() == type;
	if (res) {
		error(incorrectUseOfLink);
	}
	return res;
}
