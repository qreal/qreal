#include "editorViewScene.h"
#include "math.h"

#include <QtGui/QGraphicsTextItem>
#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsDropShadowEffect>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>

#include "editorViewMVIface.h"
#include "editorView.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

EditorViewScene::EditorViewScene(QObject *parent)
		: QGraphicsScene(parent)
		, mLastCreatedWithEdge(NULL)
		, mRightButtonPressed(false)
		, mHighlightNode(NULL)
		, mWindow(NULL)
		, mPrevParent(0)
		, mMouseMovementManager(NULL)
		, mActionSignalMapper(new QSignalMapper(this))
		, mTimer(new QTimer(this))
		, mShouldReparentItems(false)
{
	mNeedDrawGrid = SettingsManager::value("ShowGrid").toBool();
	mWidthOfGrid = static_cast<double>(SettingsManager::value("GridWidth").toInt()) / 100;
	mRealIndexGrid = SettingsManager::value("IndexGrid").toInt();

	mNeedDrawGrid = SettingsManager::value("ShowGrid").toBool();
	mWidthOfGrid = static_cast<double>(SettingsManager::value("GridWidth").toInt()) / 100;
	mRealIndexGrid = SettingsManager::value("IndexGrid").toInt();

	setItemIndexMethod(NoIndex);
	setEnabled(false);

	connect(mTimer, SIGNAL(timeout()), this, SLOT(getObjectByGesture()));
}

void EditorViewScene::drawForeground(QPainter *painter, QRectF const &rect)
{
	QPointF const point = sceneRect().topLeft();
	foreach (QPixmap *pixmap, mForegroundPixmaps) {
		painter->drawPixmap(point, *pixmap);
	}
	QGraphicsScene::drawForeground(painter, rect);
}

void EditorViewScene::putOnForeground(QPixmap *pixmap)
{
	if (!mForegroundPixmaps.contains(pixmap)) {
		mForegroundPixmaps.push_back(pixmap);
	}
}

void EditorViewScene::deleteFromForeground(QPixmap *pixmap)
{
	mForegroundPixmaps.removeAll(pixmap);
	update();
}

EditorViewScene::~EditorViewScene()
{
	delete mActionSignalMapper;
	delete mMouseMovementManager;
}

void EditorViewScene::drawIdealGesture()
{
	mMouseMovementManager->drawIdealPath();
}

void EditorViewScene::printElementsOfRootDiagram()
{
	mMouseMovementManager->setGesturesPainter(mWindow->gesturesPainter());
	mMouseMovementManager->printElements();
}

void EditorViewScene::initMouseMoveManager()
{
	if (!mMVIface || !mMVIface->graphicalAssistApi()) {
		return;
	}

	qReal::Id const diagram = mMVIface->rootId();

	if (!mainWindow() || mainWindow()->activeDiagram() != diagram) {
		// If it is not our diagram, do nothing. We will init mouse manager when our diagram will be activated
		return;
	}

	if (diagram == Id()) {
		// Root diagram is not set, for example, current tab is disabled. No need
		// to do anything with mouse manager.
		return;
	}
	QList<qReal::Id> elements = mWindow->manager()->elements(diagram);
	delete mMouseMovementManager;
	mMouseMovementManager = new MouseMovementManager(elements,
		mWindow->manager(), mWindow->gesturesPainter());
	connect(mWindow, SIGNAL(currentIdealGestureChanged()), this, SLOT(drawIdealGesture()));
	connect(mWindow, SIGNAL(gesturesShowed()), this, SLOT(printElementsOfRootDiagram()));
}

void EditorViewScene::drawGrid(QPainter *painter, const QRectF &rect)
{
	int const indexGrid = SettingsManager::value("IndexGrid").toInt();
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
	foreach (QGraphicsView *view, views()) {
		view->setEnabled(enabled);
	}
}

void EditorViewScene::clearScene()
{
	foreach (QGraphicsItem *item, items()) {
		// looks really insane, but some elements were alreadt deleted together with their parent
		if (items().contains(item)) {
			removeItem(item);
		}
	}
}

Element *EditorViewScene::getElem(qReal::Id const &id)
{
	if (id == Id::rootId()) {
		return NULL;
	}

	// FIXME: SLOW!
	QList < QGraphicsItem *> list = items();
	for (QList < QGraphicsItem *>::Iterator it = list.begin(); it != list.end(); it++) {
		if (Element *elem = dynamic_cast < Element *>(*it)) {
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
	if (mimeData->hasFormat("application/x-real-uml-data")) {
		QGraphicsScene::dragEnterEvent(event);
	} else {
		event->ignore();
	}
}

void EditorViewScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	// forming id to check if we can put draggable element to element under cursor
	QByteArray itemData = event->mimeData()->data("application/x-real-uml-data");
	QDataStream in_stream(&itemData, QIODevice::ReadOnly);
	QString uuid = "";
	in_stream >> uuid;
	Id id = Id::loadFromString(uuid);

	QList<QGraphicsItem*> elements = items(event->scenePos());

	NodeElement *node = NULL;
	foreach (QGraphicsItem *item, elements) {
		NodeElement *el = dynamic_cast<NodeElement*>(item);
		if(el != NULL){
			if (canBeContainedBy(el->id(), id)) {
				node = el;
				break;
			}
		}
	}

	if (node == NULL) {
		if (mHighlightNode != NULL) {
			mHighlightNode->erasePlaceholder(true);
		}
		return;
	}

	NodeElement *prevHighlighted = mHighlightNode;
	QGraphicsRectItem *placeholder = getPlaceholder();
	node->drawPlaceholder(placeholder, event->scenePos());
	mHighlightNode = node;
	if (prevHighlighted != mHighlightNode && prevHighlighted != NULL) {
		prevHighlighted->erasePlaceholder(true);
	}
}

NodeElement *EditorViewScene::findNewParent(QPointF newParentInnerPoint, NodeElement *node) {
	QList<QGraphicsItem *> selected = selectedItems();
	Id const &id = node->id();

	// when we select multiple elements and move them, position of mouse release event could be
	// exactly over one of them. so to prevent handling this situation as putting all others in
	// container, we check if new parent is selected right now.
	if (NULL == node->parentItem() || !selected.contains(node->parentItem())) {

		// if we want to put multiple elements in a container, we should take scene()->items()
		// and remove elements that are currently selected from it.

		// but there could be a situation that we're trying to move element and it's container
		// together. it that case we should not change parent of this element

		// delete from parents list ones that are selected right now
		// we get the first valid NodeElement
		foreach (QGraphicsItem *item, items(newParentInnerPoint)) {
			NodeElement *e = dynamic_cast<NodeElement *>(item);
			if (e != NULL && e != node && !selected.contains(item)) {
				// проверка, можно ли добавлять наш элемент в найденного родителя
				if (canBeContainedBy(e->id(), id)) {
					return e;
				}
			}

		}
	}

	return NULL;
}

QGraphicsRectItem *EditorViewScene::getPlaceholder()
{
	QGraphicsRectItem *placeholder = new QGraphicsRectItem;
	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
	QColor color(0, 150, 200, 50);
	effect->setBlurRadius(5);
	effect->setOffset(0);
	effect->setColor(Qt::black);
	placeholder->setGraphicsEffect(effect);
	placeholder->setBrush(color);
	placeholder->setPen(QPen(QColor(0, 0, 0, 0), 0));
	placeholder->setRect(0, 0, 50, 50);
	return placeholder;
}

void EditorViewScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
	if (mHighlightNode != NULL) {
		mHighlightNode->erasePlaceholder(true);
		mHighlightNode = NULL;
	}
}

void EditorViewScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_ASSERT(mWindow);  // should be initialized separately.
	// constuctor is bad for this, because the scene is created in generated .ui file

	// if there's no diagrams. create nothing
	if (!mMVIface->graphicalAssistApi()->hasRootDiagrams()) {
		return;
	}

	createElement(event->mimeData(), event->scenePos());
	if (mHighlightNode != NULL) {
		mHighlightNode->erasePlaceholder(true);
		mHighlightNode = NULL;
	}
}

bool EditorViewScene::canBeContainedBy(qReal::Id const &container, qReal::Id const &candidate) const
{
	bool allowed = false;
	foreach (qReal::Id type, mWindow->manager()->getContainedTypes(container.type())){
		allowed = allowed || mWindow->manager()->isParentOf(candidate, type);
	}
	return allowed;
}

int EditorViewScene::launchEdgeMenu(EdgeElement *edge, NodeElement *node, QPointF const &scenePos)
{
	edge->setSelected(true);

	QList<QObject*> toDelete;

	QMenu *edgeMenu = new QMenu();
	toDelete.append(edgeMenu);
	edgeMenu->addAction(mWindow->actionDeleteFromDiagram());
	edgeMenu->addAction(tr("Discard"));
	edgeMenu->addSeparator();

	QMenu *createElemMenu = new QMenu(tr("Create new element"), edgeMenu); // deleted as child of edgeMenu
	edgeMenu->addMenu(createElemMenu);

	QSignalMapper *menuSignalMapper = new QSignalMapper();
	toDelete.append(menuSignalMapper);

	foreach(PossibleEdge pEdge, edge->getPossibleEdges()){
		QString target;
		// if pEdge.first.first is parent of node->id(), then add all children of pEdge.first.second to the list
		// and vice versa

		QStringList targets;
		if (mWindow->manager()->isParentOf(node->id(), pEdge.first.first)) {
			targets << mWindow->manager()->getAllChildrenTypesOf(pEdge.first.second);
		}

		if (mWindow->manager()->isParentOf(node->id(), pEdge.first.second)) {
			targets << mWindow->manager()->getAllChildrenTypesOf(pEdge.first.first);
		}

		foreach (QString target, targets.toSet()) { // QSet is used to remove duplicates
			Id id = Id::loadFromString("qrm:/" + node->id().editor() + "/" + node->id().diagram() + "/" + target);
			QAction *element = new QAction(mWindow->manager()->friendlyName(id), createElemMenu);
			// deleted as child of createElemMenu
			createElemMenu->addAction(element);
			QObject::connect(element,SIGNAL(triggered()), menuSignalMapper, SLOT(map()));
			menuSignalMapper->setMapping(element, id.toString());
		}
	}

	mCreatePoint = scenePos;
	QObject::connect(menuSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(createElement(const QString &)));

	QPoint cursorPos = QCursor::pos();
	QAction *executed = edgeMenu->exec(cursorPos);

	int result = 0;
	if (executed) {
		if (executed == mWindow->actionDeleteFromDiagram()) {
			result = -1;
		} else if (!(executed->text() == tr("Discard"))) {
			result = 1;
		}
	}

	foreach(QObject *object, toDelete)
		delete object;

	return result;
}

qReal::Id EditorViewScene::createElement(const QString &str)
{
	qReal::Id result = createElement(str, mCreatePoint);
	mLastCreatedWithEdge = getElem(result);
	return result;
}

qReal::Id EditorViewScene::createElement(const QString &str, QPointF const &scenePos, bool searchForParents)
{
	Id typeId = Id::loadFromString(str);
	Id objectId(typeId.editor(),typeId.diagram(),typeId.element(),QUuid::createUuid().toString());

	QByteArray data;
	QMimeData *mimeData = new QMimeData();
	QDataStream stream(&data, QIODevice::WriteOnly);
	QString mimeType = QString("application/x-real-uml-data");
	QString uuid = objectId.toString();
	QString pathToItem = Id::rootId().toString();
	QString name = "(" + mWindow->manager()->friendlyName(typeId) + ")";
	QPointF pos = QPointF(0, 0);
	bool isFromLogicalModel = false;
	stream << uuid;
	stream << pathToItem;
	stream << name;
	stream << pos;
	stream << isFromLogicalModel;

	mimeData->setData(mimeType, data);
	createElement(mimeData, scenePos, searchForParents);
	delete mimeData;

	return objectId;
}

void EditorViewScene::createElement(const QMimeData *mimeData, QPointF const &scenePos, bool searchForParents)
{
	QByteArray itemData = mimeData->data("application/x-real-uml-data");
	QDataStream in_stream(&itemData, QIODevice::ReadOnly);

	QString uuid = "";
	QString pathToItem = "";
	QString name = "";
	QPointF pos;
	bool isFromLogicalModel = false;
	in_stream >> uuid;
	in_stream >> pathToItem;
	in_stream >> name;
	in_stream >> pos;
	in_stream >> isFromLogicalModel;

	Element *newParent = NULL;
	Element *e = NULL;

	// TODO: make it simpler
	Id id = Id::loadFromString(uuid);

	if (searchForParents) {
		// if element is node then we should look for parent for him
		e = mWindow->manager()->graphicalObject(id);
		if (dynamic_cast<NodeElement*>(e)) { // check if e is node
			foreach (QGraphicsItem *item, items(scenePos)) {
				NodeElement *el = dynamic_cast<NodeElement*>(item);
				if (el && canBeContainedBy(el->id(), id)) {
					newParent = el;
					break;
				}
			}
		}

		if(newParent && dynamic_cast<NodeElement*>(newParent)){
			if (!canBeContainedBy(newParent->id(), id)) {
				QString text;
				text += "Element of type \"" + id.element() + "\" can not be a child of \"" + newParent->id().element() + "\"";
				QMessageBox::critical(0, "Error!", text);
				return;
			}

			//temporary solution for chaotic changes of coordinates of created elements with edge menu
			if (dynamic_cast<EdgeElement*>(newParent)) {
				newParent = NULL;
			}
		}

	}

	QPointF const position = !newParent ? scenePos : newParent->mapToItem(newParent, newParent->mapFromScene(scenePos));

	Id parentId = newParent ? newParent->id() : mMVIface->rootId();

	//inserting new node into edge
	Id insertedNodeId = mMVIface->graphicalAssistApi()->createElement(parentId, id, isFromLogicalModel, name, position);
	if (dynamic_cast<NodeElement*>(e)) {
		insertNodeIntoEdge(insertedNodeId, parentId, isFromLogicalModel, scenePos);
	}

	NodeElement *parentNode = dynamic_cast<NodeElement*>(newParent);
	if (parentNode != NULL) {
		Element *nextNode = parentNode->getPlaceholderNextElement();
		if (nextNode != NULL) {
			mMVIface->graphicalAssistApi()->stackBefore(id, nextNode->id());
		}
	}

	if (e) {
		delete e;
	}

	emit elementCreated(id);
}

void EditorViewScene::insertNodeIntoEdge(qReal::Id const &insertedNodeId, qReal::Id const &parentId, bool isFromLogicalModel,QPointF const &scenePos)
{
	foreach (QGraphicsItem *item, items(scenePos)) {
		EdgeElement *edge = dynamic_cast<EdgeElement*>(item);
		if(edge && edge->isDividable()){// check if item is an edge and the edge is dissectable
			NodeElement *previouslyConnectedTo = edge->dst();
			if (previouslyConnectedTo) {//check has edge dst
				edge->removeLink(previouslyConnectedTo);
				previouslyConnectedTo->delEdge(edge);

				mMVIface->graphicalAssistApi()->setTo(edge->id(), insertedNodeId);
				Id const newEdge(edge->id().editor(), edge->id().diagram(), edge->id().element(), QUuid::createUuid().toString());
				Id realParentId = (parentId == Id::rootId()) ? mMVIface->rootId() : parentId;

				mMVIface->graphicalAssistApi()->createElement(realParentId, newEdge, isFromLogicalModel, "flow", scenePos);
				mMVIface->graphicalAssistApi()->setFrom(newEdge, insertedNodeId);
				mMVIface->graphicalAssistApi()->setTo(newEdge, previouslyConnectedTo->id());

				previouslyConnectedTo->connectLinksToPorts();
				break;
			}
		}
	}
}

void EditorViewScene::copy()
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);

	QList<NodeElement*> nodes = getNodesForCopying();
	QList<NodeData> nodesData;
	foreach (NodeElement* node, nodes) {
		nodesData << node->data();
	}
	QList<EdgeData> edgesData = getEdgesData(nodes);

	stream << nodesData;
	stream << edgesData;

	QMimeData* mimeData = new QMimeData();
	mimeData->setData("application/x-real-uml-model-data", data);

	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setMimeData(mimeData);
}

QList<NodeElement*> EditorViewScene::getNodesForCopying()
{
	QList<NodeElement*> nodes;
	foreach (QGraphicsItem* item, selectedItems()) {
		NodeElement* node = dynamic_cast<NodeElement*>(item);
		if (node && !selectedItems().contains(node->parentItem())) {
			nodes << node;
		}
	}
	foreach (NodeElement* node, nodes) {
		addChildren(node, nodes);
	}

	return nodes;
}

void EditorViewScene::addChildren(NodeElement* node, QList<NodeElement*> &nodes)
{
	foreach (QGraphicsItem* item, node->childItems()) {
		NodeElement* child = dynamic_cast<NodeElement*>(item);
		if (child && !nodes.contains(child)) {
			nodes << child;
			addChildren(child, nodes);
		}
	}
}

QList<EdgeData> EditorViewScene::getEdgesData(QList<NodeElement*> const &nodes)
{
	QList<EdgeData> edgesData;
	foreach (QGraphicsItem* item, selectedItems()) {
		EdgeElement* edge = dynamic_cast<EdgeElement*>(item);
		if (edge && nodes.contains(edge->src()) && nodes.contains(edge->dst())) {
			edgesData << edge->data();
		}
	}
	return edgesData;
}

void EditorViewScene::paste()
{
	QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();

	QByteArray data = mimeData->data("application/x-real-uml-model-data");
	QDataStream stream(&data, QIODevice::ReadOnly);

	QList<NodeData> nodesData;
	QList<EdgeData> edgesData;

	stream >> nodesData;
	stream >> edgesData;

	if (nodesData.isEmpty()) {
		return;
	}

	QPointF offset = getMousePos() - nodesData[0].pos;

	QHash<Id, Id> copiedIds;

	while (!nodesData.isEmpty()) {
		NodeData& nextToPaste = nodesData[0];
		Id copy = pasteNode(nextToPaste, copiedIds, offset);
		copiedIds.insert(nextToPaste.id, copy);
		nodesData.removeAll(nextToPaste);
	}

	foreach (EdgeData data, edgesData) {
		pasteEdge(data, copiedIds, offset);
	}
}

Id EditorViewScene::pasteNode(NodeData const &nodeData,
		QHash<Id, Id> const &copiedIds, QPointF const &offset)
{
	QPointF newPos = nodeData.pos;
	if (!copiedIds.contains(nodeData.parentId)) {
		newPos += offset;
	}

	Id typeId = nodeData.id.type();
	Id newId = createElement(typeId.toString(), newPos);
	NodeElement* newNode = dynamic_cast<NodeElement*>(getElem(newId));

	mMVIface->graphicalAssistApi()->setProperties(newId, nodeData.properties);

	newNode->setGeometry(nodeData.contents.translated(newPos));
	newNode->storeGeometry();

	if (!copiedIds.contains(nodeData.parentId)) {
		mMVIface->graphicalAssistApi()->changeParent(newId, Id::rootId(), newPos);
	} else {
		mMVIface->graphicalAssistApi()->changeParent(
				newId, copiedIds[nodeData.parentId], newPos);
	}

	return newId;
}

Id EditorViewScene::pasteEdge(EdgeData const &edgeData
		, QHash<Id, Id> const &copiedIds, QPointF const &offset)
{
	Id typeId = edgeData.id.type();
	Id newId = createElement(typeId.toString(), QPointF());

	Id newSrcId = copiedIds[edgeData.srcId];
	Id newDstId = copiedIds[edgeData.dstId];

	QPointF newPos = edgeData.pos + offset;
	mMVIface->graphicalAssistApi()->setPosition(newId, newPos);
	mMVIface->graphicalAssistApi()->setConfiguration(newId, edgeData.configuration);

	mMVIface->graphicalAssistApi()->setFrom(newId, newSrcId);
	mMVIface->graphicalAssistApi()->setTo(newId, newDstId);

	mMVIface->graphicalAssistApi()->setFromPort(newId, edgeData.portFrom);
	mMVIface->graphicalAssistApi()->setToPort(newId, edgeData.portTo);

	EdgeElement* newEdge = dynamic_cast<EdgeElement*>(getElem(newId));
	newEdge->connectToPort();

	return newId;
}

Element *EditorViewScene::getLastCreated()
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
	} else if (isArrow(event->key())) {
		moveSelectedItems(event->key());
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

inline bool EditorViewScene::isArrow(int key)
{
	return key == Qt::Key_Left || key == Qt::Key_Right
			|| key == Qt::Key_Down || key == Qt::Key_Up;
}

void EditorViewScene::moveSelectedItems(int direction)
{
	QPointF offset = offsetByDirection(direction);
	if (offset == QPointF(0, 0)) {
		return;
	}

	foreach (QGraphicsItem* item, selectedItems()) {
		QPointF newPos = item->pos();
		if (!item->parentItem()) {
			newPos += offset;
		}

		Element* element = dynamic_cast<Element*>(item);
		if (element) {
			mMVIface->graphicalAssistApi()->setPosition(element->id(), newPos);
		}
		element->setPos(newPos);

		NodeElement* node = dynamic_cast<NodeElement*>(item);
		if (node) {
			node->alignToGrid();
		}
	}
}

QPointF EditorViewScene::offsetByDirection(int direction)
{
	int offset = arrowMoveOffset;
	if (SettingsManager::value("ActivateGrid").toBool()) {
		offset = SettingsManager::value("IndexGrid").toInt();
	}
	switch (direction) {
		case Qt::Key_Left:
			return QPointF(-offset, 0);
		case Qt::Key_Right:
			return QPointF(offset, 0);
		case Qt::Key_Down:
			return QPointF(0, offset);
		case Qt::Key_Up:
			return QPointF(0, -offset);
		default:
			qDebug() << "Incorrect direction";
			return QPointF(0, 0);
	}
}

void EditorViewScene::createGoToSubmenu(QMenu * const goToMenu, QString const &name, qReal::IdList const &ids) const
{
	QMenu *menu = goToMenu->addMenu(name);
	foreach (Id element, ids) {
		QAction *action = menu->addAction(mMVIface->logicalAssistApi()->logicalRepoApi().name(element));
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
		foreach (Id elementId, mMVIface->logicalAssistApi()->logicalRepoApi().logicalElements(type)) {
			if (alreadyConnectedElements.contains(elementId)) {
				continue;
			}
			QAction *action = addConnectionMenu->addAction(mMVIface->logicalAssistApi()->logicalRepoApi().name(elementId));
			connect(action, SIGNAL(triggered()), slot);
			QList<QVariant> tag;
			tag << element->logicalId().toVariant() << elementId.toVariant();
			action->setData(tag);
		}
	}

	foreach (Id diagram, connectableDiagrams) {
		Id diagramType = mMVIface->logicalAssistApi()->editorManager().findElementByType(diagram.element());
		QString name = mMVIface->logicalAssistApi()->editorManager().friendlyName(diagramType);
		QString editorName = mMVIface->logicalAssistApi()->editorManager().friendlyName(Id(diagramType.editor()));
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
		QAction *action = disconnectMenu->addAction(mMVIface->logicalAssistApi()->logicalRepoApi().name(elementId));
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->logicalId().toVariant() << elementId.toVariant();
		action->setData(tag);
	}
}

void EditorViewScene::createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const
{
	if (mainWindow()->showConnectionRelatedMenus()) {
		// menu items "connect to"
		// TODO: move to elements, they can call the model and API themselves
		createAddConnectionMenu(element, contextMenu, tr("Add connection")
				, mWindow->manager()->getConnectedTypes(element->id().type())
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId())
				, mMVIface->logicalAssistApi()->diagramsAbleToBeConnectedTo(element->logicalId())
				, SLOT(connectActionTriggered())
				);

		createDisconnectMenu(element, contextMenu, tr("Disconnect")
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId())
				, mMVIface->logicalAssistApi()->logicalRepoApi().incomingConnections(element->logicalId())
				, SLOT(disconnectActionTriggered())
				);

		createAddConnectionMenu(element, contextMenu, tr("Add usage")
				, mWindow->manager()->getUsedTypes(element->id().type())
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId())
				, mMVIface->logicalAssistApi()->diagramsAbleToBeUsedIn(element->logicalId())
				, SLOT(addUsageActionTriggered())
				);

		createDisconnectMenu(element, contextMenu, tr("Delete usage")
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId())
				, mMVIface->logicalAssistApi()->logicalRepoApi().incomingUsages(element->logicalId())
				, SLOT(deleteUsageActionTriggered())
				);

		QMenu * const goToMenu = contextMenu.addMenu(tr("Go to"));

		createGoToSubmenu(goToMenu, tr("Forward connection"), mMVIface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId()));
		createGoToSubmenu(goToMenu, tr("Backward connection"), mMVIface->logicalAssistApi()->logicalRepoApi().incomingConnections(element->logicalId()));
		createGoToSubmenu(goToMenu, tr("Uses"), mMVIface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId()));
		createGoToSubmenu(goToMenu, tr("Used in"), mMVIface->logicalAssistApi()->logicalRepoApi().incomingUsages(element->logicalId()));
	}
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Let scene update selection and perform other operations
	QGraphicsScene::mousePressEvent(event);

	if (event->button() == Qt::LeftButton) {
		QGraphicsItem *item = itemAt(event->scenePos());
		ElementTitle *title = dynamic_cast < ElementTitle *>(item);

		if (title) { // check whether we accidently clicked on a title or not
			item = item->parentItem();
		}

		if (item) {
			mPrevParent = item->parentItem();
			mPrevPosition = item->pos();
		}

	} else {
		if (event->button() == Qt::RightButton) {
			mTimer->stop();
			mMouseMovementManager->mousePress(event->scenePos());
			mRightButtonPressed = true;
		}
	}
	redraw();

	mShouldReparentItems = (selectedItems().size() > 0);
	mCurrentMousePos = event->scenePos();
}

void EditorViewScene::initContextMenu(Element *e, const QPointF &pos)
{
	QMenu menu;

	if (!e) {
		mWindow->actionDeleteFromDiagram()->setEnabled(false);
		mWindow->actionCopyElementsOnDiagram()->setEnabled(false);
	}

	menu.addAction(mWindow->actionDeleteFromDiagram());
	menu.addAction(mWindow->actionCopyElementsOnDiagram());
	menu.addAction(mWindow->actionPasteOnDiagram());

	if (e) {
		QList<ContextMenuAction*> elementActions = e->contextMenuActions();

		if (!elementActions.isEmpty()) {
			menu.addSeparator();
		}

		foreach (ContextMenuAction*action, elementActions) {
			action->setEventPos(e->mapFromScene(pos));
			menu.addAction(action);

			connect(action, SIGNAL(triggered()), mActionSignalMapper, SLOT(map()),
					Qt::UniqueConnection);
			mActionSignalMapper->setMapping(action, action->text() + "###" + e->id().toString());
		}
		menu.addSeparator();
		createConnectionSubmenus(menu, e);
	}

	menu.exec(QCursor::pos());

	mWindow->actionDeleteFromDiagram()->setEnabled(true);
	mWindow->actionCopyElementsOnDiagram()->setEnabled(true);

}

void EditorViewScene::getObjectByGesture()
{
	mTimer->stop();
	qReal::Id id = mMouseMovementManager->getObject();
	if (id.element() != "")
		createElement(id.toString(), mMouseMovementManager->pos());
	deleteGesture();
}

void EditorViewScene::getLinkByGesture(NodeElement *parent, const NodeElement &child)
{
	EditorInterface const *const editorInterface = mainWindow()->manager()->editorInterface(child.id().editor());

	QList<PossibleEdge> edges = parent->getPossibleEdges();
	QList<QString> allLinks;
	foreach (PossibleEdge const &possibleEdge, edges) {
		if (possibleEdge.first.second.editor() == child.id().editor()
				&& possibleEdge.first.second.diagram() == child.id().diagram()
				&& editorInterface->isParentOf(child.id().diagram(), possibleEdge.first.second.element(), child.id().diagram(), child.id().element()))
		{
			allLinks.push_back(possibleEdge.second.second.toString());
		}
	}
	if (!allLinks.empty()) {
		if (allLinks.count() == 1) {
			createEdge(allLinks.at(0));
		} else {
			createEdgeMenu(allLinks);
		}
	}
}

void EditorViewScene::createEdgeMenu(const QList<QString> &ids)
{
	QMenu *edgeMenu = new QMenu();
	QSignalMapper *menuSignalMapper = new QSignalMapper(this);
	foreach (QString id, ids) {
		QAction *element = new QAction(id, edgeMenu);
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
	QPointF start = mMouseMovementManager->firstPoint();
	QPointF end = mMouseMovementManager->lastPoint();
	NodeElement *child = dynamic_cast <NodeElement *> (getElemAt(end));
	Id id = createElement(idStr, start);
	Element *edgeElement = getElem(id);
	EdgeElement *edge = dynamic_cast <EdgeElement *> (edgeElement);
	QPointF endPos = edge->mapFromItem(child, child->getNearestPort(end));
	edge->placeEndTo(endPos);
	edge->connectToPort();
}

void EditorViewScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);

	Element *element = getElemAt(event->scenePos());

	if (mShouldReparentItems) {
		QList<QGraphicsItem *> const list = selectedItems();
		foreach(QGraphicsItem *item, list) {
			sendEvent(item, event);
		}
		mShouldReparentItems = false; // in case there'll be 2 consecutive release events
	}

	if (event->button() == Qt::RightButton && !(mMouseMovementManager->pathIsEmpty())) {
		mMouseMovementManager->mouseMove(event->scenePos());
		mRightButtonPressed = false;
		drawGesture();
		EdgeElement * const edgeElement = dynamic_cast<EdgeElement *>(element);
		if (edgeElement != NULL) {
			if (event->buttons() & Qt::LeftButton ) {
				edgeElement->breakPointHandler(element->mapFromScene(event->scenePos()));
				return;
			}
		}
		if (!mMouseMovementManager->wasMoving()) {
			deleteGesture();
			if (element && !element->isSelected()) {
				element->setSelected(true);
			}
			initContextMenu(element, event->scenePos());
			clearSelection();
			return;
		}

		QPointF const start = mMouseMovementManager->firstPoint();
		QPointF const end = mMouseMovementManager->lastPoint();
		NodeElement *parent = dynamic_cast<NodeElement *>(getElemAt(start));
		NodeElement *child = dynamic_cast<NodeElement *>(getElemAt(end));
		if (parent && child && mMouseMovementManager->isEdgeCandidate()) {
			getLinkByGesture(parent, *child);
			deleteGesture();
		} else {
			mTimer->start(SettingsManager::value("gestureDelay").toInt());
		}
		return;
	}

	if (!element) {
		return;
	}

	redraw();
}

void EditorViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	mCurrentMousePos = event->scenePos();

	// button isn't recognized while mouse moves
	if (mRightButtonPressed) {
		mMouseMovementManager->mouseMove(event->scenePos());
		drawGesture();
	} else {
		QGraphicsScene::mouseMoveEvent(event);
	}
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
			IdList outgoingLinks = mMVIface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId());
			if (outgoingLinks.size() > 0) {
				IdList graphicalIdsOfOutgoingLinks = mMVIface->graphicalAssistApi()->graphicalIdsByLogicalId(outgoingLinks[0]);
				if (graphicalIdsOfOutgoingLinks.size() > 0) {
					mainWindow()->activateItemOrDiagram(graphicalIdsOfOutgoingLinks[0]);
				}
			} else {
				IdList diagrams = mMVIface->logicalAssistApi()->diagramsAbleToBeConnectedTo(element->logicalId());
				if (!diagrams.isEmpty()) {
					Id diagramType = mMVIface->logicalAssistApi()->editorManager().findElementByType(diagrams[0].element());
					mMVIface->logicalAssistApi()->createConnected(element->logicalId(), diagramType);
				}
			}
		}
	}

	QGraphicsScene::mouseDoubleClickEvent(event);
}

Element *EditorViewScene::getElemAt(QPointF const &position)
{
	foreach (QGraphicsItem *item, items(position)) {
		Element *e = dynamic_cast<Element *>(item);
		if (e) {
			return e;
		}
	}
	return NULL;
}

qReal::Id EditorViewScene::rootItemId() const
{
	return mMVIface->rootId();
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
		mMVIface->logicalAssistApi()->connect(source, destination);
	} else {
		mMVIface->logicalAssistApi()->createConnected(source, destination);
	}
}

void EditorViewScene::addUsageActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	if (!action->text().startsWith("New ")) {
		mMVIface->logicalAssistApi()->addUsage(source, destination);
	} else {
		mMVIface->logicalAssistApi()->createUsed(source, destination);
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
	mMVIface->logicalAssistApi()->disconnect(source, destination);
}

void EditorViewScene::deleteUsageActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	QList<QVariant> connection = action->data().toList();
	Id source = connection[0].value<Id>();
	Id destination = connection[1].value<Id>();
	mMVIface->logicalAssistApi()->deleteUsage(source, destination);
}

void EditorViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (mNeedDrawGrid) {
		mWidthOfGrid = SettingsManager::value("GridWidth").toDouble() / 100;

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
	QLineF line = mMouseMovementManager->newLine();
	QGraphicsLineItem *item = new QGraphicsLineItem(line);
	double size = mGesture.size() * 0.1;
	double color_ratio = pow(fabs(sin(size)), 1.5);
	QColor penColor(255 * color_ratio, 255 * (1 - color_ratio), 255);
	item->setPen(penColor);
	addItem(item);
	mGesture.push_back(item);
}

void EditorViewScene::deleteGesture()
{
	foreach (QGraphicsItem *item, mGesture) {
		removeItem(item);
	}
	mGesture.clear();
	mMouseMovementManager->clear();
}

void EditorViewScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
	if (wheelEvent->delta() > 0) {
		emit zoomIn();
	} else {
		emit zoomOut();
	}
	wheelEvent->accept();
	return;
}

void EditorViewScene::redraw()
{
	if (mNeedDrawGrid) {
		invalidate();
	}
}

void EditorViewScene::highlight(Id const &graphicalId, bool exclusive, QColor const &color)
{
	if (exclusive) {
		foreach (Element *element, mHighlightedElements) {
			element->setGraphicsEffect(NULL);
		}
	}

	Element *elem = getElem(graphicalId);
	if (!elem) {
		return;
	}

	QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect();
	effect->setColor(color);
	effect->setEnabled(true);

	elem->setGraphicsEffect(effect);
	mHighlightedElements.insert(elem);
}

void EditorViewScene::dehighlight(Id const &graphicalId)
{
	Element *elem = getElem(graphicalId);
	if (!elem) {
		return;
	}

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

void EditorViewScene::selectAll()
{
	foreach (QGraphicsItem *element, items()) {
		element->setSelected(true);
	}
}
