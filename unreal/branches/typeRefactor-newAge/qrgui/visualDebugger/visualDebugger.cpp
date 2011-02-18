#include "visualDebugger.h"

#include <QtCore/QSettings>

#include <QEventLoop>
#include <QTimer>
#include <QMessageBox>

#include "propertyeditorproxymodel.h"
#include "../models/models.h"

#include "../view/editorview.h"
#include "../umllib/uml_element.h"

using namespace qReal;

VisualDebugger::VisualDebugger(models::GraphicalModelAssistApi const &modelApi)
	: mModelApi(modelApi)
{
	mEffect = new QGraphicsColorizeEffect();
	mDebugColor = Qt::red;
	mEffect->setColor(mDebugColor);
	mCurrentElem = NULL;
	mCurrentId = ROOT_ID;
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
	if (NULL == mEditor || mCurrentId == ROOT_ID || mEditor == editor) {
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

UML::Element* VisualDebugger::findBeginNode(QString name)
{
	int i = 0;
	int count = mEditor->mvIface()->scene()->items().count();
	UML::Element *elem = NULL;
	while (i < count) {
		elem = dynamic_cast<UML::Element *>(mEditor->mvIface()->scene()->items().at(i));
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
	return ROOT_ID;
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
	return mModelApi.graphicalRepoApi().to(id) != ROOT_ID;
}

VisualDebugger::ErrorType VisualDebugger::doFirstStep(UML::Element *elem)
{
	if (!elem) {
		return VisualDebugger::missingBeginNode;
	}
	mCurrentElem = elem;

	mEffect = new QGraphicsColorizeEffect();
	mEffect->setColor(mDebugColor);
	mEffect->setEnabled(true);

	dynamic_cast<QGraphicsItem *>(mCurrentElem)->setGraphicsEffect(mEffect);
	mCurrentId = mCurrentElem->id();
	return VisualDebugger::noErrors;
}

void VisualDebugger::doStep(Id id)
{
	mEffect->setEnabled(true);
	mCurrentId = id;
	mCurrentElem = mEditor->mvIface()->scene()->getElem(id);
	dynamic_cast<QGraphicsItem *>(mCurrentElem)->setGraphicsEffect(mEffect);

	UML::Element *elem = dynamic_cast<UML::NodeElement *>(mCurrentElem);
	if (elem) {
		if (elem->id().element().compare("Action") == 0) {
			processAction();
		}
	}
}

void VisualDebugger::deinitialize()
{
	mEffect->setEnabled(false);
	mCurrentId = ROOT_ID;
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
	QSettings settings("SPbSU", "QReal");
	setTimeout(settings.value("debuggerTimeout", 750).toInt());
	setDebugColor(settings.value("debugColor").toString());

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
			if (validLinkId != ROOT_ID) {
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
	QSettings settings("SPbSU", "QReal");
	setDebugColor(settings.value("debugColor").toString());

	if (mCurrentElem == NULL && mCurrentId == ROOT_ID) {
		if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
			return *mErrorReporter;
		}
	} else {
		mBlockParser->setErrorReporter(mErrorReporter);

		UML::Element *elem = dynamic_cast<UML::NodeElement *>(mCurrentElem);
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
				if (validLinkId != ROOT_ID) {
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
