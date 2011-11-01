#include "visualDebugger.h"

#include <QEventLoop>
#include <QTimer>
#include <QFile>

#include "../mainwindow/propertyEditorProxyModel.h"
#include "../models/models.h"

#include "../view/editorView.h"
#include "../umllib/element.h"
#include "../../qrkernel/exception/exception.h"

using namespace qReal;

VisualDebugger::VisualDebugger(models::LogicalModelAssistApi const &logicalModelApi
							   , models::GraphicalModelAssistApi const &graphicalModelApi
							   , qReal::gui::MainWindowInterpretersInterface &interpretersInterface)
	: mInterpretersInterface(interpretersInterface)
	, mLogicalModelApi(logicalModelApi)
	, mGraphicalModelApi(graphicalModelApi)
	, mCurrentElem(NULL)
	, mError(VisualDebugger::noErrors)
	, mCurrentId(Id::rootId())
	, mBlockParser(new BlockParser(interpretersInterface.errorReporter()))
	, mTimeout(750)
	, mDebugType(VisualDebugger::noDebug)
	, mHasCodeGenerationError(false)
	, mHasNotEndWithFinalNode(false)
	, mCodeFileName("code.c")
	, mWorkDir("")
{}

VisualDebugger::~VisualDebugger()
{
	delete mBlockParser;
}

QVariant VisualDebugger::getProperty(Id id, QString propertyName)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().property(
					mGraphicalModelApi.logicalId(mEditor->mvIface()->scene()->getElem(id)->id()), propertyName);
	}
}

bool VisualDebugger::canDebug(DebugType type)
{
	switch (type) {
	case VisualDebugger::singleStepDebug:
		return mDebugType != VisualDebugger::fullDebug && mDebugType != VisualDebugger::debugWithDebugger;
		break;
	case VisualDebugger::fullDebug:
		return mDebugType == VisualDebugger::noDebug;
		break;
	case VisualDebugger::debugWithDebugger:
		return mDebugType != VisualDebugger::fullDebug && mDebugType != VisualDebugger::singleStepDebug;
		break;
	default:
		return false;
		break;
	}
}

void VisualDebugger::setDebugType(VisualDebugger::DebugType type)
{
	mDebugType = type;
}

void VisualDebugger::setTimeout(int timeout)
{
	mTimeout = timeout;
}

void VisualDebugger::setDebugColor(QString color)
{
	mDebugColor = QColor(color);
}

void VisualDebugger::setEditor(EditorView *editor) {
	if (NULL == mEditor || mCurrentId == Id::rootId() || mEditor == editor)
	{
		mEditor = editor;
	} else {
		mError = VisualDebugger::someDiagramIsRunning;
	}
}

void VisualDebugger::error(ErrorType e)
{
	switch (e) {
	case missingBeginNode:
		mInterpretersInterface.errorReporter()->addCritical("The diagram doesn't have Initial Node");
		break;
	case missingEndNode:
		mInterpretersInterface.errorReporter()->addCritical("The diagram doesn't end with Final Node");
		break;
	case missingEndOfLinkNode:
		mInterpretersInterface.errorReporter()->addCritical("The diagram cann't end with edge", mCurrentId);
		break;
	case endWithNotEndNode:
		mInterpretersInterface.errorReporter()->addWarning("There are no links from this node"
														   " and it mismatches Final Node", mCurrentId);
		break;
	case missingValidLink:
		mInterpretersInterface.errorReporter()->addCritical("The condition doesn't have valid link", mCurrentId);
		break;
	case someDiagramIsRunning:
		mInterpretersInterface.errorReporter()->addCritical("Some diagram is already under debug");
		mError = VisualDebugger::noErrors;
		return;
		break;
	case codeGenerationError:
		mInterpretersInterface.errorReporter()->addCritical("Code generation failed");
		mHasCodeGenerationError = true;
	case noErrors:
		break;
	}
	deinitialize();
}

Element* VisualDebugger::findBeginNode(QString name)
{
	int i = 0;
	int count = mEditor->mvIface()->scene()->items().count();
	Element *elem = NULL;
	while (i < count) {
		elem = dynamic_cast<Element *>(mEditor->mvIface()->scene()->items().at(i));
		if (elem && elem->id().element().compare(name) == 0) {
			break;
		}
		i++;
	}
	if (i == count) {
		error(VisualDebugger::missingBeginNode);
		return NULL;
	}
	return elem;
}

Id VisualDebugger::findValidLink()
{
	IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);
	QString conditionStr = getProperty(mCurrentId, "condition").toString();
	int pos = 0;
	bool condition = mBlockParser->parseCondition(conditionStr, pos, mCurrentId);
	for (int i = 0; i < outLinks.count(); i++) {
		bool type = getProperty(outLinks.at(i), "type").toBool();
		if (type == condition) {
			return outLinks.at(i);
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

bool VisualDebugger::isFinalNode(Id id)
{
	IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(id);
	return outLinks.count() == 0 && id.element().compare("BlockFinalNode") == 0;
}

bool VisualDebugger::hasEndOfLinkNode(Id id)
{
	return mLogicalModelApi.logicalRepoApi().to(id) != Id::rootId();
}

VisualDebugger::ErrorType VisualDebugger::doFirstStep(Element *elem)
{
	if (!elem) {
		return VisualDebugger::missingBeginNode;
	}
	mCurrentElem = elem;
	mCurrentId = mCurrentElem->id();

	highlight(mCurrentId);

	return VisualDebugger::noErrors;
}

void VisualDebugger::doStep(Id id)
{
	mCurrentId = id;
	mCurrentElem = mEditor->mvIface()->scene()->getElem(id);

	highlight(mCurrentId);

	Element *elem = dynamic_cast<NodeElement *>(mCurrentElem);
	if (elem) {
		if (elem->id().element().compare("Action") == 0) {
			processAction();
		}
	}
}

void VisualDebugger::deinitialize()
{
	dehighlight();
	mCurrentId = Id::rootId();
	mCurrentElem = NULL;
	mEditor = NULL;
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
	setTimeout(SettingsManager::value("debuggerTimeout", 750).toInt());
	setDebugColor(SettingsManager::value("debugColor").toString());


	if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
		return;
	}

	mBlockParser->setErrorReporter(mInterpretersInterface.errorReporter());

	IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);

	while (outLinks.count() > 0) {
		pause(mTimeout);

		if (mCurrentElem->id().element().compare("ConditionNode") == 0) {
			Id validLinkId = findValidLink();
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

	mInterpretersInterface.errorReporter()->addInformation("Debug finished successfully");
	deinitialize();
	return;
}

void VisualDebugger::debugSingleStep()
{
	mDebugType = VisualDebugger::singleStepDebug;
	setDebugColor(SettingsManager::value("debugColor").toString());

	if (mCurrentElem == NULL && mCurrentId == Id::rootId()) {
		if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
			return;
		}
	} else {
		mBlockParser->setErrorReporter(mInterpretersInterface.errorReporter());

		Element *elem = dynamic_cast<NodeElement *>(mCurrentElem);
		if (elem) {
			if (mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId).count() == 0) {
				if (!isFinalNode(mCurrentId)) {
					error(VisualDebugger::endWithNotEndNode);
					return;
				}
				deinitialize();
				mInterpretersInterface.errorReporter()->addInformation("Debug (single step) finished successfully");
				return ;
			}

			if (mCurrentElem->id().element().compare("ConditionNode") == 0) {
				Id validLinkId = findValidLink();
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
				doStep(mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId).at(0));
				if (mBlockParser->hasErrors()) {
					deinitialize();
					return;
				}
			}
			mInterpretersInterface.errorReporter()->addInformation("Debug (single step) finished successfully");
			return;
		} else {
			if (!hasEndOfLinkNode(mCurrentId)) {
				error(VisualDebugger::missingEndOfLinkNode);
				return;
			}
			doStep(mLogicalModelApi.logicalRepoApi().to(mCurrentId));
			if (mBlockParser->hasErrors()) {
				deinitialize();
				return;
			}
		}
	}

	mInterpretersInterface.errorReporter()->addInformation("Debug (single step) finished successfully");
	return;
}

void VisualDebugger::generateCode()
{
	mHasCodeGenerationError = false;

	setCodeFileName(SettingsManager::value("codeFileName", "code.c").toString());
	setWorkDir(SettingsManager::value("debugWorkingDirectory", "").toString());

	QFile codeFile(mWorkDir + "/" + mCodeFileName);
	codeFile.open(QIODevice::WriteOnly);

	codeFile.write("void main(int argc, char* argv[]) {\n");
	Element *curElem = findBeginNode("InitialNode");
	if (curElem != NULL) {
		generateCode(curElem, codeFile);
		codeFile.write("}");
		codeFile.close();
		return;
	}
	codeFile.close();
	error(codeGenerationError);
	return;
}

void VisualDebugger::generateCode(Element *elem, QFile &codeFile)
{
	Element *curElem = dynamic_cast<NodeElement *>(elem);
	if (curElem && elem->id().element().compare("InitialNode") != 0) {
		if (elem->id().element().compare("Action") == 0) {
			QString code = getProperty(curElem->id(), "process").toString();
			if (code.mid(0,4).compare("var ") == 0) {
				codeFile.write(code.mid(4).toAscii());
			} else {
				codeFile.write(code.toAscii());
			}

			codeFile.write("\n");
			if (mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).count() != 0) {
				Id nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).at(0);
				generateCode(mEditor->mvIface()->scene()->getElem(nextEdge), codeFile);
			} else {
				error(VisualDebugger::missingEndNode);
				error(codeGenerationError);
				return;
			}
		} else {
			if (elem->id().element().compare("ConditionNode") == 0) {
				codeFile.write("if (");
				codeFile.write(getProperty(curElem->id(), "condition").toByteArray());
				codeFile.write(") {\n");
				IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id());
				Id falseEdge = falseEdge.rootId();
				Id trueEdge = trueEdge.rootId();
				for (int i=0; i<outLinks.count(); i++) {
					bool type = getProperty(outLinks.at(i), "type").toBool();
					if (type) {
						trueEdge = outLinks.at(i);
					} else {
						falseEdge = outLinks.at(i);
					}
				}
				if (trueEdge == trueEdge.rootId()) {
					error(VisualDebugger::missingValidLink);
					error(codeGenerationError);
					return;
				}
				generateCode(mEditor->mvIface()->scene()->getElem(trueEdge), codeFile);
				codeFile.write("}\n");
				if (falseEdge != falseEdge.rootId()) {
					codeFile.write("else {\n");
					generateCode(mEditor->mvIface()->scene()->getElem(falseEdge), codeFile);
					codeFile.write("}\n");
				}
			}
		}
	} else {
		if (elem->id().element().compare("InitialNode") != 0) {
			Id nextNode = mLogicalModelApi.logicalRepoApi().to(elem->id());
			generateCode(mEditor->mvIface()->scene()->getElem(nextNode), codeFile);
		} else {
			Id nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).at(0);
			generateCode(mEditor->mvIface()->scene()->getElem(nextEdge), codeFile);
		}
	}

	return;
}

void VisualDebugger::createIdByLineCorrelation()
{
	mHasNotEndWithFinalNode = false;
	int line = 2;
	Element *curElem = findBeginNode("InitialNode");
	mIdByLineCorrelation[1] = curElem->id();
	createIdByLineCorrelation(curElem, line);
}

void VisualDebugger::createIdByLineCorrelation(Element *elem, int& line)
{
	Element *curElem = dynamic_cast<NodeElement *>(elem);
	if (curElem && elem->id().element().compare("InitialNode") != 0) {
		if (elem->id().element().compare("Action") == 0) {
			mIdByLineCorrelation[line] = elem->id();
			line++;
			if (mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).count() != 0) {
				Id nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).at(0);
				createIdByLineCorrelation(mEditor->mvIface()->scene()->getElem(nextEdge), line);
			} else {
				error(missingEndNode);
				mHasNotEndWithFinalNode = true;
			}
		} else {
			if (elem->id().element().compare("ConditionNode") == 0) {
				mIdByLineCorrelation[line] = elem->id();
				line++;
				IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id());
				Id falseEdge = falseEdge.rootId();
				Id trueEdge = trueEdge.rootId();
				for (int i=0; i<outLinks.count(); i++) {
					bool type = getProperty(outLinks.at(i), "type").toBool();
					if (type) {
						trueEdge = outLinks.at(i);
					} else {
						falseEdge = outLinks.at(i);
					}
				}
				createIdByLineCorrelation(mEditor->mvIface()->scene()->getElem(trueEdge), line);
				line++;
				if (falseEdge != falseEdge.rootId()) {
					line++;
					createIdByLineCorrelation(mEditor->mvIface()->scene()->getElem(falseEdge), line);
					line++;
				}
			} else {
				mIdByLineCorrelation[line] = elem->id();
			}
		}
	} else {
		if (elem->id().element().compare("InitialNode") != 0) {
			Id nextNode  = mLogicalModelApi.logicalRepoApi().to(elem->id());
			createIdByLineCorrelation(mEditor->mvIface()->scene()->getElem(nextNode), line);
		} else {
			Id nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).at(0);
			createIdByLineCorrelation(mEditor->mvIface()->scene()->getElem(nextEdge), line);
		}
	}
}

QList<int>* VisualDebugger::computeBreakpoints()
{
	QList<int> *breakpoints = new QList<int>();
	int line=1;
	Id curId = mIdByLineCorrelation[line];
	while (mIdByLineCorrelation[line].element().compare("BlockFinalNode") != 0) {

		while (mIdByLineCorrelation.contains(line) &&
			   curId.toString().compare(mIdByLineCorrelation[line].toString()) == 0) {
			line++;
		}
		breakpoints->append(line-1);

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

void VisualDebugger::highlight(Id id)
{
	mInterpretersInterface.highlight(id);
}

void VisualDebugger::dehighlight()
{
	mInterpretersInterface.dehighlight();
}

bool VisualDebugger::canBuild()
{
	return !mHasCodeGenerationError;
}

void VisualDebugger::setCodeFileName(QString name)
{
	mCodeFileName = name;
}

bool VisualDebugger::canComputeBreakpoints()
{
	return !mHasNotEndWithFinalNode;
}

void VisualDebugger::setWorkDir(QString path)
{
	mWorkDir = path;
}
