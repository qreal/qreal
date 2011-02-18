#include "visualDebugger.h"

#include <QtCore/QSettings>

#include <QEventLoop>
#include <QTimer>
#include <QFile>

#include "propertyeditorproxymodel.h"
#include "../models/models.h"

#include "../view/editorview.h"
#include "../umllib/uml_element.h"
#include "../kernel/exception/exception.h"

using namespace qReal;

VisualDebugger::VisualDebugger(models::LogicalModelAssistApi const &logicalModelApi, models::GraphicalModelAssistApi const &graphicalModelApi)
	: mLogicalModelApi(logicalModelApi), mGraphicalModelApi(graphicalModelApi)
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

QVariant VisualDebugger::getProperty(Id id, QString propertyName) {
	if (mLogicalModelApi.isLogicalId(id)) {
		try {
			return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
		} catch (qReal::Exception e) {
			return mGraphicalModelApi.graphicalRepoApi().property(
				mGraphicalModelApi.graphicalIdsByLogicalId(id).at(0), propertyName);
		}
	} else {
		try {
			return mGraphicalModelApi.graphicalRepoApi().property(id, propertyName);
		} catch (qReal::Exception e) {
			return mLogicalModelApi.logicalRepoApi().property(
				mGraphicalModelApi.logicalId(mEditor->mvIface()->scene()->getElem(id)->id()), propertyName);
		}
	}
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
	QString editorName = mGraphicalModelApi.name(idRootItem);
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

UML::Element* VisualDebugger::findEndNode(QString name)
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
	IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);
	QString conditionStr = getProperty(mCurrentId, "condition").toString();
	int pos=0;
	bool condition = mBlockParser->parseCondition(conditionStr, pos, mCurrentId);
	for (int i=0; i<outLinks.count(); i++) {
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
	mBlockParser->parseProcess(getProperty(mCurrentId, "process").toString(), pos, mCurrentId);
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

	IdList outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);

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

		doStep(mLogicalModelApi.logicalRepoApi().to(mCurrentId));
		if (mBlockParser->hasErrors()) {
			deinitialize();
			return *mErrorReporter;
		}

		outLinks = mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId);
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

	if (mCurrentElem == NULL && mCurrentId == Id::rootId()) {
		if (VisualDebugger::noErrors != doFirstStep(findBeginNode("InitialNode"))) {
			return *mErrorReporter;
		}
	} else {
		mBlockParser->setErrorReporter(mErrorReporter);

		UML::Element *elem = dynamic_cast<UML::NodeElement *>(mCurrentElem);
		if (elem) {
			if (mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId).count() == 0) {
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
				doStep(mLogicalModelApi.logicalRepoApi().outgoingLinks(mCurrentId).at(0));
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
			doStep(mLogicalModelApi.logicalRepoApi().to(mCurrentId));
			if (mBlockParser->hasErrors()) {
				deinitialize();
				return *mErrorReporter;
			}
		}
	}

	mErrorReporter->addInformation("Debug (single step) finished successfully");
	return *mErrorReporter;
}

void VisualDebugger::generateCode() {
	QFile codeFile("code.c");
	codeFile.open(QIODevice::WriteOnly);
	
	codeFile.write("void main(int argc, char* argv[]) {\n");
	UML::Element *curElem = findBeginNode("InitialNode");
	generateCode(curElem, codeFile);
	codeFile.write("}");
	codeFile.close();
}

void VisualDebugger::generateCode(UML::Element *elem, QFile &codeFile) {
	UML::Element *curElem = dynamic_cast<UML::NodeElement *>(elem);
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
			Id nextNode  = mLogicalModelApi.logicalRepoApi().to(elem->id());
			generateCode(mEditor->mvIface()->scene()->getElem(nextNode), codeFile);
		} else {
			Id nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).at(0);
			generateCode(mEditor->mvIface()->scene()->getElem(nextEdge), codeFile);
		}
	}
}

void VisualDebugger::createIdByLineCorrelation() {
	int line = 2;
	UML::Element *curElem = findBeginNode("InitialNode");
	mIdByLineCorrelation[1] = curElem->id();
	createIdByLineCorrelation(curElem, line);
	curElem = findEndNode("BlockFinalNode");
	mIdByLineCorrelation[line] = curElem->id();
}

void VisualDebugger::createIdByLineCorrelation(UML::Element *elem, int& line) {
	UML::Element *curElem = dynamic_cast<UML::NodeElement *>(elem);
	if (curElem && elem->id().element().compare("InitialNode") != 0) {
		if (elem->id().element().compare("Action") == 0) {
			mIdByLineCorrelation[line] = elem->id();
			line++;
			if (mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).count() != 0) {
				Id nextEdge = mLogicalModelApi.logicalRepoApi().outgoingLinks(curElem->id()).at(0);
				createIdByLineCorrelation(mEditor->mvIface()->scene()->getElem(nextEdge), line);
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

QList<int>* VisualDebugger::computeBreakpoints() {
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

Id VisualDebugger::getIdByLine(int line) {
	return mIdByLineCorrelation[line];
}

void VisualDebugger::lightElement(Id id) {
	mEffect->setEnabled(true);

	UML::Element *elemToLight = mEditor->mvIface()->scene()->getElem(id);
	dynamic_cast<QGraphicsItem *>(elemToLight)->setGraphicsEffect(mEffect);
}
