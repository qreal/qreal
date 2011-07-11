#include "editorviewscene.h"
#include "math.h"

#include <QGraphicsTextItem>
#include <QtGui>
#include <QtCore/QDebug>
#include <QGraphicsItem>

#include "editorviewmviface.h"
#include "editorview.h"
#include "../mainwindow/mainwindow.h"

using namespace qReal;

EditorViewScene::EditorViewScene(QObject * parent)
	:  QGraphicsScene(parent)
	, mWindow(NULL)
	, mPrevParent(0)
	, mShouldReparentItems(false)
{
	mNeedDrawGrid = SettingsManager::value("ShowGrid", true).toBool();
	mWidthOfGrid = static_cast<double>(SettingsManager::value("GridWidth", 10).toInt()) / 100;
	mRealIndexGrid = SettingsManager::value("IndexGrid", 30).toInt();

	mNeedDrawGrid = SettingsManager::value("ShowGrid", true).toBool();
	mWidthOfGrid = static_cast<double>(SettingsManager::value("GridWidth", 10).toInt()) / 100;
	mRealIndexGrid = SettingsManager::value("IndexGrid", 30).toInt();

	setItemIndexMethod(NoIndex);
	setEnabled(false);
	mRightButtonPressed = false;
	mActionSignalMapper = new QSignalMapper(this);
}

EditorViewScene::~EditorViewScene()
{
	delete mActionSignalMapper;
}

void EditorViewScene::drawIdealGesture()
{
	mouseMovementManager->drawIdealPath();
}

void EditorViewScene::printElementsOfRootDiagram()
{
	mouseMovementManager->setGesturesPainter(mWindow->gesturesPainter());
	mouseMovementManager->printElements();
}

void EditorViewScene::initMouseMoveManager()
{
	if (!mv_iface || !mv_iface->graphicalAssistApi())
		return;
	qReal::Id diagram = mv_iface->graphicalAssistApi()->idByIndex(mWindow->rootIndex());
	if (diagram == Id())
		// Root diagram is not set, for example, current tab is disabled. No need
		// to do anything with mouse manager.
		return;
	QList<qReal::Id> elements = mWindow->manager()->elements(diagram);
	mouseMovementManager = new MouseMovementManager(elements,
			mWindow->manager(), mWindow->gesturesPainter());
	connect(mWindow, SIGNAL(currentIdealGestureChanged()), this, SLOT(drawIdealGesture()));
	connect(mWindow, SIGNAL(gesturesShowed()), this, SLOT(printElementsOfRootDiagram()));
}

void EditorViewScene::drawGrid(QPainter *painter, const QRectF &rect)
{
	int const indexGrid = SettingsManager::value("IndexGrid", 50).toInt();
	qreal const sceneX = rect.x();
	qreal const sceneY = rect.y();

	int const indent = 100; // should be enough

	int startX = static_cast<int>(sceneX + 10) / indexGrid * indexGrid - indent;
	int endX = static_cast<int>(sceneX + rect.width() - 10) / indexGrid * indexGrid + indent;
	int startY = static_cast<int>(sceneY + 10) / indexGrid * indexGrid - indent;
	int endY = static_cast<int>(sceneY + rect.height() - 10) / indexGrid * indexGrid + indent;
	for (int i = startX; i <= endX; i = i + indexGrid) {
		QLineF line(i, startY, i, endY);
		painter->drawLine(line);
	}
	for (int i = startY; i <= endY; i = i + indexGrid) {
		QLineF line(startX, i, endX, i);
		painter->drawLine(line);
	}
}

double EditorViewScene::realIndexGrid()
{
	return mRealIndexGrid;
}

void EditorViewScene::setRealIndexGrid(double newIndexGrid)
{
	mRealIndexGrid = newIndexGrid;
}

void EditorViewScene::setEnabled(bool enabled)
{
	foreach (QGraphicsView *view, views())
		view->setEnabled(enabled);
}

void EditorViewScene::clearScene()
{
	foreach (QGraphicsItem *item, items())
		// looks really insane, but some elements were alreadt deleted together with their parent
		if (items().contains(item))
			removeItem(item);
}

Element * EditorViewScene::getElem(qReal::Id const &id)
{
	if (id == Id::rootId())
		return NULL;

	// FIXME: SLOW!
	QList < QGraphicsItem * > list = items();
	for (QList < QGraphicsItem * >::Iterator it = list.begin(); it != list.end(); it++) {
		if (Element * elem = dynamic_cast < Element * >(*it)) {
			if (elem->id() == id) {
				return elem;
			}
		}
	}
	return NULL;
}

void EditorViewScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasFormat("application/x-real-uml-data"))
		QGraphicsScene::dragEnterEvent(event);
	else
		event->ignore();
}

void EditorViewScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
}

void EditorViewScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
}

bool EditorViewScene::canBeContainedBy(qReal::Id container, qReal::Id candidate)
{
	Q_UNUSED(container);
	Q_UNUSED(candidate); // TODO: update xml descriptions to remove
	return true;

	/*
	bool allowed = false;
	foreach (qReal::Id type, mWindow->manager()->getContainedTypes(container.type())){
		if (candidate.element() ==  type.editor())
			allowed = true;
	}
	return allowed;
	*/
}

void EditorViewScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_ASSERT(mWindow); // should be initialized separately.
	// constuctor is bad for this, because the scene is created in generated .ui file

	// if there's no diagrams. create nothing
	if (!mv_iface->graphicalAssistApi()->hasRootDiagrams())
		return;

	createElement(event->mimeData(),event->scenePos());
}

int EditorViewScene::launchEdgeMenu(EdgeElement* edge, NodeElement* node, QPointF scenePos)
{
	edge->setSelected(true);

	QList<QObject*> toDelete;

	QMenu *edgeMenu = new QMenu();
	toDelete.append(edgeMenu);
	edgeMenu->addAction(mWindow->actionDeleteFromDiagram());
	edgeMenu->addAction(tr("Discard"));
	edgeMenu->addSeparator();

	QMenu *createElemMenu = new QMenu(tr("Create new element"), edgeMenu);
	toDelete.append(createElemMenu);
	edgeMenu->addMenu(createElemMenu);

	QSignalMapper *menuSignalMapper = new QSignalMapper(this);
	toDelete.append(menuSignalMapper);

	foreach(PossibleEdge pEdge, edge->getPossibleEdges())
	{
		QString target;
		if (pEdge.first.first.element() == node->id().element())
			target = pEdge.first.second.element();
		else if ((pEdge.first.second.element() == node->id().element()) && (!pEdge.second.first))
			target = pEdge.first.first.element();
		else continue;

		QAction* element = new QAction(target, createElemMenu);
		createElemMenu->addAction(element);
		toDelete.append(element);
		QObject::connect(element,SIGNAL(triggered()), menuSignalMapper,SLOT(map()));
		menuSignalMapper->setMapping(element, "qrm:/"+node->id().editor()+"/"+node->id().diagram()+"/"+target);
	}

	mCreatePoint = scenePos;
	QObject::connect(menuSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(createElement(const QString &)));

	QAction* executed;
	QPoint cursorPos = QCursor::pos();
	executed = edgeMenu->exec(cursorPos);

	int result = 0;
	if (executed) {
		if (executed == mWindow->actionDeleteFromDiagram())
			result = -1;
		else if (!(executed->text() == tr("Discard")))
			result = +1;
	}

	//	Cleaning.
	//	foreach(QObject *object, toDelete)
	//		delete object;

	return result;
}

qReal::Id EditorViewScene::createElement(const QString &str)
{
	qReal::Id result = createElement(str, mCreatePoint);
	mLastCreatedWithEdge = getElem(result);
	return result;
}

qReal::Id EditorViewScene::createElement(const QString &str, QPointF scenePos)
{
	Id typeId = Id::loadFromString(str);
	Id objectId(typeId.editor(),typeId.diagram(),typeId.element(),QUuid::createUuid().toString());

	QByteArray data;
	QMimeData *mimeData = new QMimeData();
	QDataStream stream(&data, QIODevice::WriteOnly);
	QString mimeType = QString("application/x-real-uml-data");
	QString uuid = objectId.toString();
	QString pathToItem = Id::rootId().toString();
	QString name = "(anonymous something)";
	QPointF pos = QPointF(0, 0);
	bool isFromLogicalModel = false;
	stream << uuid;
	stream << pathToItem;
	stream << name;
	stream << pos;
	stream << isFromLogicalModel;

	mimeData->setData(mimeType, data);
	createElement(mimeData, scenePos);
	delete mimeData;

	return objectId;
}

void EditorViewScene::createElement(const QMimeData *mimeData, QPointF scenePos)
{
	QByteArray itemData = mimeData->data("application/x-real-uml-data");
	QDataStream in_stream(&itemData, QIODevice::ReadOnly);

	QString uuid = "";
	QString pathToItem = "";
	QString name;
	QPointF pos;
	bool isFromLogicalModel;
	in_stream >> uuid;
	in_stream >> pathToItem;
	in_stream >> name;
	in_stream >> pos;
	in_stream >> isFromLogicalModel;

	Element *newParent = NULL;

	// TODO: make it simpler
	qReal::Id id = qReal::Id::loadFromString(uuid);
	Element* e = mWindow->manager()->graphicalObject(id);

	// TODO: what is it??
	if (dynamic_cast<NodeElement*>(e))
		newParent = getElemAt(scenePos);

	if (e) {
		delete e;
	}

	if(newParent){
		if (!canBeContainedBy(newParent->id(), id)){
			QMessageBox::critical(0, "Error!", "[some text]");
			return;
		}

		//temporary solution for chaotic changes of coordinates of created elements with edge menu
		EdgeElement* edge = dynamic_cast<EdgeElement*>(newParent);
		if (edge)
			newParent = NULL;
	}

	QPointF const position = !newParent ? scenePos : newParent->mapToItem(newParent, newParent->mapFromScene(scenePos));

	Id parentId = newParent ? newParent->id() : mv_iface->rootId();
	id = mv_iface->graphicalAssistApi()->createElement(parentId, id, isFromLogicalModel, name, position);
	emit elementCreated(id);
}

void EditorViewScene::copy()
{
	mCopiedNode = dynamic_cast<NodeElement*>(selectedItems()[0]);
}

void EditorViewScene::paste()
{
	if (mCopiedNode)
		mCopiedNode->copyAndPlaceOnDiagram();
	else
		qDebug() << "paste attempt on NULL";
}

Element* EditorViewScene::getLastCreated()
{
	return mLastCreatedWithEdge;
}

void EditorViewScene::keyPressEvent(QKeyEvent *event)
{
	if (dynamic_cast<QGraphicsTextItem*>(focusItem())) {
		// Forward event to text editor
		QGraphicsScene::keyPressEvent(event);
	} else if (event->key() == Qt::Key_Delete) {
		// Delete selected elements from scene
		mainWindow()->deleteFromScene();
	} else if (event->matches(QKeySequence::Paste)) {
		paste();
	} else if (event->matches(QKeySequence::Copy)) {
		copy();
	} else
		QGraphicsScene::keyPressEvent(event);
}

void EditorViewScene::createGoToSubmenu(QMenu * const goToMenu, QString const &name, qReal::IdList const &ids) const
{
	QMenu *menu = goToMenu->addMenu(name);
	foreach (Id element, ids) {
		QAction *action = menu->addAction(mv_iface->logicalAssistApi()->logicalRepoApi().name(element));
		connect(action, SIGNAL(triggered()), SLOT(goToActionTriggered()));
		action->setData(element.toVariant());
	}
}

void EditorViewScene::createAddConnectionMenu(Element const * const element
		, QMenu &contextMenu, QString const &menuName
		, IdList const &connectableTypes, IdList const &alreadyConnectedElements
		, IdList const &connectableDiagrams, const char *slot) const
{
	QMenu *addConnectionMenu = contextMenu.addMenu(menuName);

	foreach (Id type, connectableTypes) {
		foreach (Id elementId, mv_iface->logicalAssistApi()->logicalRepoApi().logicalElements(type)) {
			if (alreadyConnectedElements.contains(elementId))
				continue;
			QAction *action = addConnectionMenu->addAction(mv_iface->logicalAssistApi()->logicalRepoApi().name(elementId));
			connect(action, SIGNAL(triggered()), slot);
			QList<QVariant> tag;
			tag << element->logicalId().toVariant() << elementId.toVariant();
			action->setData(tag);
		}
	}

	foreach (Id diagram, connectableDiagrams) {
		Id diagramType = mv_iface->logicalAssistApi()->editorManager().findElementByType(diagram.element());
		QString name = mv_iface->logicalAssistApi()->editorManager().friendlyName(diagramType);
		QString editorName = mv_iface->logicalAssistApi()->editorManager().friendlyName(Id(diagramType.editor()));
		QAction *action = addConnectionMenu->addAction("New " + editorName + "/" + name);
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->logicalId().toVariant() << diagramType.toVariant();
		action->setData(tag);
	}
}

void EditorViewScene::createDisconnectMenu(Element const * const element
										   , QMenu &contextMenu, QString const &menuName
										   , IdList const &outgoingConnections, IdList const &incomingConnections
										   , const char *slot) const
{
	QMenu *disconnectMenu = contextMenu.addMenu(menuName);
	IdList list = outgoingConnections;
	list.append(incomingConnections);

	foreach (Id elementId, list) {
		QAction *action = disconnectMenu->addAction(mv_iface->logicalAssistApi()->logicalRepoApi().name(elementId));
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->logicalId().toVariant() << elementId.toVariant();
		action->setData(tag);
	}
}

void EditorViewScene::createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const
{
	// menu items "connect to"
	// TODO: move to elements, they can call the model and API themselves
	createAddConnectionMenu(element, contextMenu, tr("Add connection")
							, mWindow->manager()->getConnectedTypes(element->id().type())
							, mv_iface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId())
							, mv_iface->logicalAssistApi()->diagramsAbleToBeConnectedTo(element->logicalId())
							, SLOT(connectActionTriggered())
							);

	createDisconnectMenu(element, contextMenu, tr("Disconnect")
						 , mv_iface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId())
						 , mv_iface->logicalAssistApi()->logicalRepoApi().incomingConnections(element->logicalId())
						 , SLOT(disconnectActionTriggered())
						 );

	createAddConnectionMenu(element, contextMenu, tr("Add usage")
							, mWindow->manager()->getUsedTypes(element->id().type())
							, mv_iface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId())
							, mv_iface->logicalAssistApi()->diagramsAbleToBeUsedIn(element->logicalId())
							, SLOT(addUsageActionTriggered())
							);

	createDisconnectMenu(element, contextMenu, tr("Delete usage")
						 , mv_iface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId())
						 , mv_iface->logicalAssistApi()->logicalRepoApi().incomingUsages(element->logicalId())
						 , SLOT(deleteUsageActionTriggered())
						 );

	QMenu *goToMenu = contextMenu.addMenu(tr("Go to"));

	createGoToSubmenu(goToMenu, tr("Forward connection"), mv_iface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId()));
	createGoToSubmenu(goToMenu, tr("Backward connection"), mv_iface->logicalAssistApi()->logicalRepoApi().incomingConnections(element->logicalId()));
	createGoToSubmenu(goToMenu, tr("Uses"), mv_iface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId()));
	createGoToSubmenu(goToMenu, tr("Used in"), mv_iface->logicalAssistApi()->logicalRepoApi().incomingUsages(element->logicalId()));
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Let scene update selection and perform other operations
	QGraphicsScene::mousePressEvent(event);

	if( event->button() == Qt::LeftButton ){
		QGraphicsItem *item = itemAt(event->scenePos());
		ElementTitle *title = dynamic_cast < ElementTitle * >(item);

		if (title) // check whether we accidently clicked on a title or not
			item = item->parentItem();

		if (item) {
			mPrevParent = item->parentItem();
			mPrevPosition = item->pos();
		}

	} else if (event->button() == Qt::RightButton) {
		Element *e = getElemAt(event->scenePos());
		//	if (!e) {
		mouseMovementManager->addPoint(event->scenePos());
		mRightButtonPressed = true;
		//		return;
		//	}
		if (e && !e->isSelected()) {
			clearSelection();
			e->setSelected(true);
		}

		// Menu belongs to scene handler because it can delete elements.
		// We cannot allow elements to commit suicide.
		//if (e)
		//	initContextMenu(e, event->scenePos());
	}
	redraw();

	mShouldReparentItems = (selectedItems().size() > 0);
}

void EditorViewScene::initContextMenu(Element *e, const QPointF &pos)
{
	QMenu menu;
	menu.addAction(mWindow->actionDeleteFromDiagram());
	QList<ContextMenuAction*> elementActions = e->contextMenuActions();

	if (!elementActions.isEmpty())
		menu.addSeparator();

	foreach (ContextMenuAction* action, elementActions) {
		action->setEventPos(e->mapFromScene(pos));
		menu.addAction(action);

		connect(action, SIGNAL(triggered()), mActionSignalMapper, SLOT(map()),
				Qt::UniqueConnection);
		mActionSignalMapper->setMapping(action, action->text() + "###" + e->id().toString());
	}
	menu.addSeparator();
	createConnectionSubmenus(menu, e);
	menu.exec(QCursor::pos());
}

void EditorViewScene::getObjectByGesture()
{
	deleteGesture();
	QPointF start = mouseMovementManager->firstPoint();
	QPointF end = mouseMovementManager->lastPoint();
	NodeElement * parent = dynamic_cast <NodeElement * > (getElemAt(start));
	NodeElement * child = dynamic_cast <NodeElement * > (getElemAt(end));
	if (parent && child)
	{
		getLinkByGesture(parent, *child);
	}
	else
	{
		qReal::Id id = mouseMovementManager->getObject();
		if (id.element() != "")
			createElement(id.toString(), mouseMovementManager->pos());
	}
}

void EditorViewScene::getLinkByGesture(NodeElement * parent, const NodeElement &child)
{
	EditorInterface const * const editorInterface = mainWindow()->manager()->editorInterface(child.id().editor());

	QList<PossibleEdge> edges = parent->getPossibleEdges();
	QList<QString> allLinks;
	foreach (PossibleEdge const &possibleEdge, edges)
	{
		if (possibleEdge.first.second.editor() == child.id().editor()
			&& possibleEdge.first.second.diagram() == child.id().diagram()
			&& editorInterface->isParentOf(child.id().diagram(), possibleEdge.first.second.element(), child.id().diagram(), child.id().element()))
		{
			allLinks.push_back(possibleEdge.second.second.toString());
		}
	}
	if (!allLinks.empty())
	{
		if (allLinks.count() == 1)
			createEdge(allLinks.at(0));
		else
			createEdgeMenu(allLinks);
	}
}

void EditorViewScene::createEdgeMenu(const QList<QString> &ids)
{
	QMenu * edgeMenu = new QMenu();
	QSignalMapper * menuSignalMapper = new QSignalMapper(this);
	foreach (QString id, ids)
	{
		QAction * element = new QAction(id, edgeMenu);
		edgeMenu->addAction(element);
		QObject::connect(element, SIGNAL(triggered()), menuSignalMapper, SLOT(map()));
		menuSignalMapper->setMapping(element, id);
	}
	QObject::connect(menuSignalMapper, SIGNAL(mapped(const QString &)), this,
					 SLOT(createEdge(QString)));
	edgeMenu->exec(QCursor::pos());
}

void EditorViewScene::createEdge(const QString & idStr)
{
	QPointF start = mouseMovementManager->firstPoint();
	QPointF end = mouseMovementManager->lastPoint();
	NodeElement * child = dynamic_cast <NodeElement * > (getElemAt(end));
	Id id = createElement(idStr, start);
	Element * edgeElement = getElem(id);
	EdgeElement * edge = dynamic_cast <EdgeElement * > (edgeElement);
	QPointF endPos = edge->mapFromItem(child, child->getNearestPort(end));
	edge->placeEndTo(endPos);
	edge->connectToPort();
}

void EditorViewScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);

	Element* element = getElemAt(event->scenePos());

	if (mShouldReparentItems) {
		QList<QGraphicsItem *> list = selectedItems();
		foreach(QGraphicsItem *item, list)
			sendEvent(item, event);
		mShouldReparentItems = false; // in case there'll be 2 consecutive release events
	}

	if (event->button() == Qt::RightButton)
	{
		mouseMovementManager->addPoint(event->scenePos());
		EdgeElement *edgeElement = dynamic_cast<EdgeElement *>(element);
		if(edgeElement != NULL)
			if (event->buttons() & Qt::LeftButton ) {
				edgeElement->breakPointHandler(element->mapFromScene(event->scenePos()));
				mRightButtonPressed = false;
				return;}
		if (mouseMovementManager->wasMoving())
			getObjectByGesture();
		else if (element)
			initContextMenu(element, event->scenePos());
		mRightButtonPressed = false;
		mouseMovementManager->clear();
		return;
	}

	if (!element)
		return;

	Element *parent = dynamic_cast <Element *>(getElem(element->id())->parent());

	if (parent) {
		if (!canBeContainedBy(parent->id(), element->id())){
			QMessageBox::critical(0, "Ololo", "can't drop it here!111");
			// fail, reparenting the element as it was before
			foreach (QGraphicsItem *item, items(event->scenePos())) {
				Element * elem = dynamic_cast < Element * >(item);
				if (elem && elem->id() == element->id()) {
					qReal::Id id = qReal::Id::rootId();
					Element * prevParent = dynamic_cast < Element * >(mPrevParent);
					if (prevParent)
						id = prevParent->id();
					if (mv_iface && mv_iface->graphicalAssistApi()) {
						mv_iface->graphicalAssistApi()->changeParent(element->id(), id, mPrevPosition);
					}
				}
			}
		}
	}
	redraw();
}

void EditorViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	mCurrentMousePos = event->scenePos();

	// button isn't recognized while mouse moves
	if (mRightButtonPressed)
	{
		mouseMovementManager->addPoint(event->scenePos());
		drawGesture();
	}
	else
		QGraphicsScene::mouseMoveEvent(event);
}

QPointF EditorViewScene::getMousePos()
{
	return mCurrentMousePos;
}

void EditorViewScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		// Double click on a title activates it
		if (ElementTitle *title = dynamic_cast<ElementTitle*>(itemAt(event->scenePos()))) {
			if (!title->hasFocus()) {  // Do not activate already activated item
				event->accept();
				title->startTextInteraction();
				return;
			}
		}
		else if (NodeElement *element = dynamic_cast<NodeElement*>(itemAt(event->scenePos()))) {
			event->accept();
			IdList outgoingLinks = mv_iface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId());
			if (outgoingLinks.size() > 0) {
				IdList graphicalIdsOfOutgoingLinks = mv_iface->graphicalAssistApi()->graphicalIdsByLogicalId(outgoingLinks[0]);
				if (graphicalIdsOfOutgoingLinks.size() > 0)
					mainWindow()->activateItemOrDiagram(graphicalIdsOfOutgoingLinks[0]);
			} else {
				IdList diagrams = mv_iface->logicalAssistApi()->diagramsAbleToBeConnectedTo(element->logicalId());
				if (!diagrams.isEmpty()) {
					Id diagramType = mv_iface->logicalAssistApi()->editorManager().findElementByType(diagrams[0].element());
					mv_iface->logicalAssistApi()->createConnected(element->logicalId(), diagramType);
				}
			}

			// Now scene is changed from outside. Being a mere mortal I do not
			// know whether it is good or not, but what is the destiny of
			// workflow after this return?
			return;
		}
	}

	QGraphicsScene::mouseDoubleClickEvent(event);
}

Element* EditorViewScene::getElemAt(QPointF const &position)
{
	foreach (QGraphicsItem *item, items(position)) {
		Element* e = dynamic_cast<Element*>(item);
		if (e)
			return e;
	}
	return NULL;
}

qReal::Id EditorViewScene::rootItemId() const
{
	return mv_iface->rootId();
}

void EditorViewScene::setMainWindow(qReal::MainWindow *mainWindow)
{
	mWindow = mainWindow;
	connect(mWindow, SIGNAL(rootDiagramChanged()), this, SLOT(initMouseMoveManager()));
//	connect(this, SIGNAL(elementCreated(qReal::Id)), mainWindow->listenerManager(), SIGNAL(objectCreated(qReal::Id)));
//	connect(mActionSignalMapper, SIGNAL(mapped(QString)), mainWindow->listenerManager(), SIGNAL(contextMenuActionTriggered(QString)));
}

qReal::MainWindow *EditorViewScene::mainWindow() const
{
	return mWindow;
}

void EditorViewScene::connectActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	if (!action->text().startsWith("New ")) {
		mv_iface->logicalAssistApi()->connect(source, destination);
	} else {
		mv_iface->logicalAssistApi()->createConnected(source, destination);
	}
}

void EditorViewScene::addUsageActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	if (!action->text().startsWith("New ")) {
		mv_iface->logicalAssistApi()->addUsage(source, destination);
	} else {
		mv_iface->logicalAssistApi()->createUsed(source, destination);
	}
}

void EditorViewScene::goToActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id target = action->data().value<Id>();
	mainWindow()->activateItemOrDiagram(target);
	return;
}

void EditorViewScene::disconnectActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	mv_iface->logicalAssistApi()->disconnect(source, destination);
}

void EditorViewScene::deleteUsageActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	mv_iface->logicalAssistApi()->deleteUsage(source, destination);
}

void EditorViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (mNeedDrawGrid) {
		mWidthOfGrid = (SettingsManager::value("GridWidth", 10).toDouble()) / 100;

		painter->setPen(QPen(Qt::black, mWidthOfGrid));
		drawGrid(painter, rect);
	}
}

void EditorViewScene::setNeedDrawGrid(bool show)
{
	mNeedDrawGrid = show;
}

bool EditorViewScene::getNeedDrawGrid()
{
	return mNeedDrawGrid;
}

void EditorViewScene::drawGesture()
{
	QLineF line = mouseMovementManager->newLine();
	QGraphicsLineItem * item = new QGraphicsLineItem(line, NULL, this);
	double size = mGesture.size() * 0.1;
	double color_ratio = pow(fabs(sin(size)), 1.5);
	QColor penColor(255 * color_ratio, 255 * (1 - color_ratio), 255);
	item->setPen(penColor);
	addItem(item);
	mGesture.push_back(item);
	int ellipseSize = 2;
	QPointF bound(ellipseSize, ellipseSize);
	QRectF rect(line.p2() + bound, line.p2() - bound);
	QGraphicsEllipseItem * ellipseItem = new QGraphicsEllipseItem(rect, NULL, this);
	ellipseItem->setPen(penColor);
	addItem(ellipseItem);
	mGesture.push_back(ellipseItem);
}

void EditorViewScene::deleteGesture()
{
	foreach (QGraphicsItem * item, mGesture) {
		removeItem(item);
	}
	mGesture.clear();
}

void EditorViewScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
	if (wheelEvent->delta() > 0)
		emit zoomIn();
	else
		emit zoomOut();
	wheelEvent->accept();
	return;
}

void EditorViewScene::redraw()
{
	if (mNeedDrawGrid)
		invalidate();
}

void EditorViewScene::highlight(Id const &graphicalId, bool exclusive)
{
	if (exclusive) {
		foreach (Element *element, mHighlightedElements) {
			element->setGraphicsEffect(NULL);
		}
	}

	Element *elem = getElem(graphicalId);
	if (!elem)
		return;

	QColor color = QColor(SettingsManager::value("debugColor", "red").toString());

	QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
	effect->setColor(color);
	effect->setEnabled(true);

	elem->setGraphicsEffect(effect);
	mHighlightedElements.insert(elem);
}

void EditorViewScene::dehighlight(Id const &graphicalId)
{
	Element *elem = getElem(graphicalId);
	if (!elem)
		return;

	elem->setGraphicsEffect(NULL);
	mHighlightedElements.remove(elem);
}

void EditorViewScene::dehighlight()
{
	foreach (Element *element, mHighlightedElements) {
		element->setGraphicsEffect(NULL);
	}
	mHighlightedElements.clear();
}
