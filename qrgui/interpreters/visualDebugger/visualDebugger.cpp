#include "visualDebugger.h"

#include <QEventLoop>
#include <QTimer>
#include <QMessageBox>

#include "propertyEditorProxyModel.h"
#include "../models/models.h"

#include "../view/editorView.h"
#include "../umllib/element.h"

using namespace qReal;

VisualDebugger::VisualDebugger(models::GraphicalModelAssistApi const &modelApi)
	: mModelApi(modelApi)
{
	mEffect = new QGraphicsColorizeEffect();
	mDebugColor = Qt::red;
	mEffect->setColor(mDebugColor);
	mCurrentElem = NULL;
	mCurrentId = Id::rootId();
	mError = VisualDebugger::noErrors;
	mErrorReporter = new gui::ErrorReporter();
	mBlockParser = new BlockParser(mErrorReporter);
	mTimeout = 750;
	mDebugType = VisualDebugger::noDebug;
}

VisualDebugger::~VisualDebugger()
{
	delete mEffect;
	delete mErrorReporter;
	delete mBlockParser;
}

bool VisualDebugger::canDebug(DebugType type)
{
	switch (type) {
	case VisualDebugger::singleStepDebug:
		return mDebugType != VisualDebugger::fullDebug;
		break;
	case VisualDebugger::fullDebug:
		return mDebugType == VisualDebugger::noDebug;
		break;
	default:
		return false;
		break;
	}
}

void VisualDebugger::setTimeout(int timeout)
{
	mTimeout = timeout;
}

void VisualDebugger::setDebugColor(QString color)
{
	mDebugColor = QColor(color);
}

void VisualDebugger::setEditor(EditorView *editor)
{
	if (NULL == mEditor || mCurrentId == Id::rootId() || mEditor == editor) {
		mEditor = editor;
	} else {
		mError = VisualDebugger::someDiagramIsRunning;
	}
}

VisualDebugger::ErrorType VisualDebugger::checkEditor()
{
	if (mError != VisualDebugger::noErrors) {
		error(VisualDebugger::someDiagramIsRunning);
		return VisualDebugger::someDiagramIsRunning;
	}
	Id idRootItem = mEditor->mvIface()->scene()->rootItemId();
	QString editorName = mModelApi.name(idRootItem);
	if (editorName.compare("(Block Diagram)") != 0) {
		error(VisualDebugger::wrongEditor);
		return VisualDebugger::wrongEditor;
	}
	return VisualDebugger::noErrors;
}

void VisualDebugger::error(ErrorType e)
{
	switch (e) {
	case missingBeginNode:
		mErrorReporter->addCritical("The diagram doesn't have Initial Node");
		break;
	case missingEndOfLinkNode:
		mErrorReporter->addCritical("The diagram cann't end with edge", mCurrentId);
		break;
	case endWithNotEndNode:
		mErrorReporter->addWarning("There are no links from this node and it mismatches Final Node",
			mCurrentId);
		break;
	case missingValidLink:
		mErrorReporter->addCritical("The condition doesn't have valid link", mCurrentId);
		break;
	case someDiagramIsRunning:
		mErrorReporter->addCritical("Some diagram is already under debug");
		mError = VisualDebugger::noErrors;
		return;
		break;
	case wrongEditor:
		mErrorReporter->addCritical("This is not Block Diagram editor");
		break;
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
	IdList outLinks = mModelApi.graphicalRepoApi().outgoingLinks(mCurrentId);
	QString conditionStr = mModelApi.graphicalRepoApi().property(mCurrentId, "condition").toString();
	int pos=0;
	bool condition = mBlockParser->parseCondition(conditionStr, pos, mCurrentId);
	for (int i=0; i<outLinks.count(); i++) {
		bool type = mModelApi.graphicalRepoApi().property(outLinks.at(i), "type").toBool();
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
	IdList outLinks = mModelApi.graphicalRepoApi().outgoingLinks(id);
	return outLinks.count() == 0 && id.element().compare("BlockFinalNode") == 0;
}

bool VisualDebugger::hasEndOfLinkNode(Id id)
{
	return mModelApi.graphicalRepoApi().to(id) != Id::rootId();
}

VisualDebugger::ErrorType VisualDebugger::doFirstStep(Element *elem)
{
	if (!elem) {
		return VisualDebugger::missingBeginNode;
	}
	mCurrentElem = elem;

	mEffect = new QGraphicsColorizeEffect();
	mEffect->setColor(mDebugColor);
	mEffect->setEnabled(true);

	mCurrentElem->setGraphicsEffect(mEffect);
	mCurrentId = mCurrentElem->id();
	return VisualDebugger::noErrors;
}

void VisualDebugger::doStep(Id id)
{
	mEffect->setEnabled(true);
	mCurrentId = id;
	mCurrentElem = mEditor->mvIface()->scene()->getElem(id);
	mCurrentElem->setGraphicsEffect(mEffect);

	if (mCurrentElem) {
		if (mCurrentElem->id().element() == "Action") {
			processAction();
		}
	}
}

void VisualDebugger::deinitialize()
{
	mEffect->setEnabled(false);
	mCurrentId = Id::rootId();
	mCurrentElem = NULL;
	mEditor = NULL;
	mError = VisualDebugger::noErrors;
	mBlockParser->clear();
	mDebugType = VisualDebugger::noDebug;
}

void VisualDebugger::clearErrorReporter()
{
	mErrorReporter = new gui::ErrorReporter();
}

void VisualDebugger::processAction()
{
	int pos = 0;
	mBlockParser->parseProcess(mModelApi.graphicalRepoApi().property(mCurrentId, "process").toString(), pos, mCurrentId);
}

gui::ErrorReporter& VisualDebugger::debug()
{
	if (VisualDebugger::noErrors != checkEditor()) {
		return *mErrorReporter;
	}

	mDebugType = VisualDebugger::fullDebug;
	setTimeout(SettingsManager::instance()->value("debuggerTimeout", 750).toInt());
	setDebugColor(SettingsManager::instance()->value("debugColor").toString());

	if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
		return *mErrorReporter;
	}

	mBlockParser->setErrorReporter(mErrorReporter);

	IdList outLinks = mModelApi.graphicalRepoApi().outgoingLinks(mCurrentId);

	while (outLinks.count() > 0) {
		pause(mTimeout);

		if (mCurrentElem->id().element().compare("ConditionNode") == 0) {
			Id validLinkId = findValidLink();
			if (mBlockParser->hasErrors()) {
				deinitialize();
				return *mErrorReporter;
			}
			if (validLinkId != Id::rootId()) {
				doStep(validLinkId);
			} else {
				return *mErrorReporter;
			}
		} else {
			doStep(outLinks.at(0));
			if (mBlockParser->hasErrors()) {
				deinitialize();
				return *mErrorReporter;
			}
		}

		pause(mTimeout);

		if (!hasEndOfLinkNode(mCurrentId)) {
			error(VisualDebugger::missingEndOfLinkNode);
			return *mErrorReporter;
		}

		doStep(mModelApi.graphicalRepoApi().to(mCurrentId));
		if (mBlockParser->hasErrors()) {
			deinitialize();
			return *mErrorReporter;
		}

		outLinks = mModelApi.graphicalRepoApi().outgoingLinks(mCurrentId);
	}

	pause(mTimeout);

	if (!isFinalNode(mCurrentId)) {
		error(VisualDebugger::endWithNotEndNode);
		return *mErrorReporter;
	}

	mErrorReporter->addInformation("Debug finished successfully");
	deinitialize();
	return *mErrorReporter;
}

gui::ErrorReporter& VisualDebugger::debugSingleStep()
{
	if (VisualDebugger::noErrors != checkEditor()) {
		return *mErrorReporter;
	}

	mDebugType = VisualDebugger::singleStepDebug;
	setDebugColor(SettingsManager::instance()->value("debugColor").toString());

	if (mCurrentElem == NULL && mCurrentId == Id::rootId()) {
		if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
			return *mErrorReporter;
		}
	} else {
		mBlockParser->setErrorReporter(mErrorReporter);

		Element *elem = dynamic_cast<NodeElement *>(mCurrentElem);
		if (elem) {
			if (mModelApi.graphicalRepoApi().outgoingLinks(mCurrentId).count() == 0) {
				if (!isFinalNode(mCurrentId)) {
					error(VisualDebugger::endWithNotEndNode);
					return *mErrorReporter;
				}
				deinitialize();
				mErrorReporter->addInformation("Debug (single step) finished successfully");
				return *mErrorReporter;
			}

			if (mCurrentElem->id().element().compare("ConditionNode") == 0) {
				Id validLinkId = findValidLink();
				if (mBlockParser->hasErrors()) {
					deinitialize();
					return *mErrorReporter;
				}
				if (validLinkId != Id::rootId()) {
					doStep(validLinkId);
				} else {
					return *mErrorReporter;
				}
			} else {
				doStep(mModelApi.graphicalRepoApi().outgoingLinks(mCurrentId).at(0));
				if (mBlockParser->hasErrors()) {
					deinitialize();
					return *mErrorReporter;
				}
			}
			mErrorReporter->addInformation("Debug (single step) finished successfully");
			return *mErrorReporter;
		} else {
			if (!hasEndOfLinkNode(mCurrentId)) {
				error(VisualDebugger::missingEndOfLinkNode);
				return *mErrorReporter;
			}
			doStep(mModelApi.graphicalRepoApi().to(mCurrentId));
			if (mBlockParser->hasErrors()) {
				deinitialize();
				return *mErrorReporter;
			}
		}
	}

	mErrorReporter->addInformation("Debug (single step) finished successfully");
	return *mErrorReporter;
}
