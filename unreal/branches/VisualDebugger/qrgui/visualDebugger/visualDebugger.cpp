#include "visualDebugger.h"

#include <QEventLoop>
#include <QTimer>
#include <QMessageBox>

#include "propertyeditorproxymodel.h"

#include "../model/model.h"
#include "../view/editorview.h"
#include "../umllib/uml_element.h"

using namespace qReal;

VisualDebugger::VisualDebugger(EditorView *editor, model::Model *model) {
	mEditor = editor;
	mModel = model;
	mEffect = new QGraphicsColorizeEffect();
	mEffect->setColor(Qt::red);
	mCurrentElem = NULL;
	mCurrentId = mCurrentId.getRootId();
}

VisualDebugger::~VisualDebugger() {
	
}

void VisualDebugger::error(ErrorType e) {
	deinitialize();
	switch (e) {
	case missingBeginNode:
		QMessageBox::critical(0,
			tr("Error"),
			tr("The diagram doesn't have Initial Node.\nCorrect this and redebug your program."),
			QMessageBox::Yes);
		break;
	case missingEndOfLinkNode:
		QMessageBox::critical(0,
			tr("Error"),
			tr("Diagram cann't end with edge.\nCorrect this and redebug your program."),
			QMessageBox::Yes);
		break;
	case endWithNotEndNode:
		QMessageBox::warning(0,
			tr("Warning"),
			tr("There are no links from this node and it mismatches Activity Test Final Node."),
			QMessageBox::Yes);
		break;
	case missingValidLink:
		QMessageBox::critical(0,
			tr("Error"),
			tr("The condition doesn't have valid link.\nCorrect this and redebug your program."),
			QMessageBox::Yes);
		break;
	case noErrors:
		break;
	}
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
	for (int i=0; i<outLinks.count(); i++) {
		QString condition = mModel->api().property(mCurrentId, "condition").toString();
		QString type = mModel->api().property(outLinks.at(i), "type").toString();
		if (type.compare(condition) == 0) {
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
	return (outLinks.count() == 0 && id.element().compare("ActivityTestFinalNode") == 0);
}

bool VisualDebugger::hasEndOfLinkNode(Id id) {
	return !(mModel->api().to(id) == id.getRootId());
}

VisualDebugger::ErrorType VisualDebugger::doFirstStep(UML::Element *elem) {
	if (!elem) {
		return missingBeginNode;
	}
	mCurrentElem = elem;
	
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
}

void VisualDebugger::deinitialize() {
	mEffect->setEnabled(false);
	mCurrentId = mCurrentId.getRootId();
	mCurrentElem = NULL;
}

void VisualDebugger::debug() {
	if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
		return;
	}
	IdList outLinks = mModel->api().outgoingLinks(mCurrentId);
	
	while (outLinks.count() > 0) {
		pause(750);
		
		if (mCurrentElem->uuid().element().compare("DecisionNode") == 0) {
			Id validLinkId = findValidLink();
			if (validLinkId != validLinkId.getRootId()) {
				doStep(validLinkId);
			} else {
				return;
			}
		} else {
			doStep(outLinks.at(0));
		}
		
		pause(750);
		
		if (!hasEndOfLinkNode(mCurrentId)) {
			error(VisualDebugger::missingEndOfLinkNode);
			return;
		}
		
		doStep(mModel->api().to(mCurrentId));
		
		outLinks = mModel->api().outgoingLinks(mCurrentId);
	}
	
	pause(750);
	
	if (!isFinalNode(mCurrentId)) {
		error(VisualDebugger::endWithNotEndNode);
		return;
	}
	
	deinitialize();
}

void VisualDebugger::debugSingleStep() {
	if (mCurrentElem == NULL && mCurrentId == mCurrentId.getRootId()) {
		if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
			return;
		}
	} else {
		UML::Element *elem = dynamic_cast<UML::NodeElement *>(mCurrentElem);
		if (elem) {
			if (mModel->api().outgoingLinks(mCurrentId).count() == 0) {
				if (!isFinalNode(mCurrentId)) {
					error(VisualDebugger::endWithNotEndNode);
					return;
				}
				deinitialize();
				return;
			}
			
			if (mCurrentElem->uuid().element().compare("DecisionNode") == 0) {
				Id validLinkId = findValidLink();
				if (validLinkId != validLinkId.getRootId()) {
					doStep(validLinkId);
				} else {
					return;
				}
			} else {
				doStep(mModel->api().outgoingLinks(mCurrentId).at(0));
			}
			return;
		} else {
			if (!hasEndOfLinkNode(mCurrentId)) {
				error(VisualDebugger::missingEndOfLinkNode);
				return;
			}
			doStep(mModel->api().to(mCurrentId));
		}
	}
}
