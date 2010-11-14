#include "visualDebugger.h"

#include <QEventLoop>
#include <QTimer>
#include <QMessageBox>

#include "propertyeditorproxymodel.h"

#include "../model/model.h"
#include "../view/editorview.h"
#include "../umllib/uml_element.h"

using namespace qReal;

VisualDebugger::VisualDebugger(model::Model *model) {
	mModel = model;
	mEffect = new QGraphicsColorizeEffect();
	mEffect->setColor(Qt::red);
	mCurrentElem = NULL;
	mCurrentId = mCurrentId.getRootId();
	mBlockParser = new blockParser();
	mError = VisualDebugger::noErrors;
}

VisualDebugger::~VisualDebugger() {
	
}

void VisualDebugger::setEditor(EditorView *editor) {
	if (NULL == mEditor || mCurrentId == mCurrentId.getRootId() || mEditor == editor) {
		mEditor = editor;
	} else {
		mError = VisualDebugger::someDiagramIsRunning;
	}
}

VisualDebugger::ErrorType VisualDebugger::checkEditor() {
	if (mError != VisualDebugger::noErrors) {
		error(VisualDebugger::someDiagramIsRunning);
		return VisualDebugger::someDiagramIsRunning;
	}
	QString editorName = mEditor->mvIface()->scene()->rootItem().data().toString();
	if (editorName.compare("(Block Diagram)") != 0) {
		error(VisualDebugger::wrongEditor);
		return VisualDebugger::wrongEditor;
	}
	return VisualDebugger::noErrors;
}

void VisualDebugger::error(ErrorType e) {
	
	switch (e) {
	case missingBeginNode:
		mErrorReporter.addCritical("The diagram doesn't have Initial Node");
		break;
	case missingEndOfLinkNode:
		mErrorReporter.addCritical("The diagram cann't end with edge", mCurrentId);
		break;
	case endWithNotEndNode:
		mErrorReporter.addWarning("There are no links from this node and it mismatches Final Node",
			mCurrentId);
		break;
	case missingValidLink:
		mErrorReporter.addCritical("The condition doesn't have valid link", mCurrentId);
		break;
	case someDiagramIsRunning:
		mErrorReporter.addCritical("Some diagram is already under debug");
		mError = VisualDebugger::noErrors;
		return;
		break;
	case wrongEditor:
		mErrorReporter.addCritical("This is not Block Diagram editor");
		break;
	case noErrors:
		break;
	}
	deinitialize();
}

UML::Element* VisualDebugger::findBeginNode(QString name) {
	int i = 0, count = mEditor->mvIface()->scene()->items().count();
	UML::Element *elem = NULL;
	while (i < count) {
		elem = dynamic_cast<UML::Element *>(mEditor->mvIface()->scene()->items().at(i));
		if (elem && elem->uuid().element().compare(name) == 0) {
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

Id VisualDebugger::findValidLink() {
	IdList outLinks = mModel->api().outgoingLinks(mCurrentId);
	QString conditionStr = mModel->api().property(mCurrentId, "condition").toString();
	int pos=0;
	bool condition = mBlockParser->parseCondition(conditionStr, pos);
	for (int i=0; i<outLinks.count(); i++) {
		bool type = mModel->api().property(outLinks.at(i), "type").toBool();
		if (type == condition) {
			return outLinks.at(i);
		}
	}
	error(VisualDebugger::missingValidLink);
	return outLinks.at(0).getRootId();
}

void VisualDebugger::pause(int time) {
	QEventLoop loop;
	QTimer::singleShot(time, &loop, SLOT(quit()));
	loop.exec();
}

bool VisualDebugger::isFinalNode(Id id) {
	IdList outLinks = mModel->api().outgoingLinks(id);
	return (outLinks.count() == 0 && id.element().compare("BlockFinalNode") == 0);
}

bool VisualDebugger::hasEndOfLinkNode(Id id) {
	return !(mModel->api().to(id) == id.getRootId());
}

VisualDebugger::ErrorType VisualDebugger::doFirstStep(UML::Element *elem) {
	if (!elem) {
		return missingBeginNode;
	}
	mCurrentElem = elem;
	
	mEffect = new QGraphicsColorizeEffect();
	mEffect->setColor(Qt::red);
	mEffect->setEnabled(true);
	
	dynamic_cast<QGraphicsItem *>(mCurrentElem)->setGraphicsEffect(mEffect);
	mCurrentId = mModel->idByIndex(mCurrentElem->index());
	return VisualDebugger::noErrors;
}

void VisualDebugger::doStep(Id id) {
	mEffect->setEnabled(true);
	mCurrentId = id;
	mCurrentElem = mEditor->mvIface()->scene()->getElem(id);
	dynamic_cast<QGraphicsItem *>(mCurrentElem)->setGraphicsEffect(mEffect);
	
	UML::Element *elem = dynamic_cast<UML::NodeElement *>(mCurrentElem);
	if (elem) {
		if (elem->uuid().element().compare("Action") == 0) {
			processAction();
		}
	}
}

void VisualDebugger::deinitialize() {
	mEffect->setEnabled(false);
	mCurrentId = mCurrentId.getRootId();
	mCurrentElem = NULL;
	mEditor = NULL;
	mError = VisualDebugger::noErrors;
}

void VisualDebugger::clearErrorReporter() {
	mErrorReporter = *(new gui::ErrorReporter());
}

void VisualDebugger::processAction() {
	int pos = 0;
	mBlockParser->parseProcess(mModel->api().property(mCurrentId, "process").toString(), pos);
}

gui::ErrorReporter VisualDebugger::debug() {
	if (VisualDebugger::noErrors != checkEditor()) {
		return mErrorReporter;
	}

	if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
		return mErrorReporter;
	}
	
	IdList outLinks = mModel->api().outgoingLinks(mCurrentId);
	
	while (outLinks.count() > 0) {
		pause(750);
		
		if (mCurrentElem->uuid().element().compare("ConditionNode") == 0) {
			Id validLinkId = findValidLink();
			if (validLinkId != validLinkId.getRootId()) {
				doStep(validLinkId);
			} else {
				return mErrorReporter;
			}
		} else {
			doStep(outLinks.at(0));
		}
		
		pause(750);
		
		if (!hasEndOfLinkNode(mCurrentId)) {
			error(VisualDebugger::missingEndOfLinkNode);
			return mErrorReporter;
		}
		
		doStep(mModel->api().to(mCurrentId));
		
		outLinks = mModel->api().outgoingLinks(mCurrentId);
	}
	
	pause(750);
	
	if (!isFinalNode(mCurrentId)) {
		error(VisualDebugger::endWithNotEndNode);
		return mErrorReporter;
	}
	
	deinitialize();
	return mErrorReporter;
}

gui::ErrorReporter VisualDebugger::debugSingleStep() {
	if (VisualDebugger::noErrors != checkEditor()) {
		return mErrorReporter;
	}

	if (mCurrentElem == NULL && mCurrentId == mCurrentId.getRootId()) {
		if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
			return mErrorReporter;
		}
	} else {
		UML::Element *elem = dynamic_cast<UML::NodeElement *>(mCurrentElem);
		if (elem) {
			if (mModel->api().outgoingLinks(mCurrentId).count() == 0) {
				if (!isFinalNode(mCurrentId)) {
					error(VisualDebugger::endWithNotEndNode);
					return mErrorReporter;
				}
				deinitialize();
				return mErrorReporter;
			}
			
			if (mCurrentElem->uuid().element().compare("ConditionNode") == 0) {
				Id validLinkId = findValidLink();
				if (validLinkId != validLinkId.getRootId()) {
					doStep(validLinkId);
				} else {
					return mErrorReporter;
				}
			} else {
				doStep(mModel->api().outgoingLinks(mCurrentId).at(0));
			}
			return mErrorReporter;
		} else {
			if (!hasEndOfLinkNode(mCurrentId)) {
				error(VisualDebugger::missingEndOfLinkNode);
				return mErrorReporter;
			}
			doStep(mModel->api().to(mCurrentId));
		}
	}
	return mErrorReporter;
}







Id VisualDebugger::findValidLink_ex() {
	IdList outLinks = mModel->api().outgoingLinks(mCurrentId);
	for (int i=0; i<outLinks.count(); i++) {
		QString condition = mModel->api().property(mCurrentId, "decision").toString();
		QString type = mModel->api().property(outLinks.at(i), "type_number").toString();
		if (type.compare(condition) == 0) {
			return outLinks.at(i);
		}
	}
	error(VisualDebugger::missingValidLink);
	return outLinks.at(0).getRootId();
}

void VisualDebugger::doStep_ex(Id id) {
	mEffect->setEnabled(true);
	mCurrentId = id;
	mCurrentElem = mEditor->mvIface()->scene()->getElem(id);
	dynamic_cast<QGraphicsItem *>(mCurrentElem)->setGraphicsEffect(mEffect);
}
