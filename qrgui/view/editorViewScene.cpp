#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include "editorViewScene.h"
#include "math.h"
#include "editorViewMVIface.h"
#include "editorView.h"
#include "../mainwindow/mainWindow.h"
#include "../dialogs/propertiesDialog.h"

using namespace qReal;

EditorViewScene::EditorViewScene(QObject *parent)
		: QGraphicsScene(parent)
		, mLastCreatedWithEdge(NULL)
		, mRightButtonPressed(false)
		, mLeftButtonPressed(false)
		, mHighlightNode(NULL)
		, mWindow(NULL)
		, mPrevParent(0)
		, mMouseMovementManager(NULL)
		, mActionSignalMapper(new QSignalMapper(this))
		, mTimer(new QTimer(this))
		, mTimerForArrowButtons(new QTimer(this))
		, mOffset(QPointF(0, 0))
		, mShouldReparentItems(false)
		, mTopLeftCorner(new QGraphicsRectItem(0, 0, 1, 1))
		, mBottomRightCorner(new QGraphicsRectItem(0, 0, 1, 1))
		, mIsSelectEvent(false)
		, mTitlesVisible(true)
{
	mNeedDrawGrid = SettingsManager::value("ShowGrid").toBool();
	mWidthOfGrid = static_cast<double>(SettingsManager::value("GridWidth").toInt()) / 100;
	mRealIndexGrid = SettingsManager::value("IndexGrid").toInt();

	setItemIndexMethod(NoIndex);
	setEnabled(false);

	initCorners();

	connect(mTimer, SIGNAL(timeout()), this, SLOT(getObjectByGesture()));
	connect(mTimerForArrowButtons, SIGNAL(timeout()), this, SLOT(updateMovedElements()));

	mSelectList = new QList<QGraphicsItem *>();
}

void EditorViewScene::addItem(QGraphicsItem *item)
{
	Element *element = dynamic_cast<Element *>(item);
	if (element) {
		element->setTitlesVisible(mTitlesVisible);
	}
	QGraphicsScene::addItem(item);
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
}

EditorViewScene::~EditorViewScene()
{
	delete mActionSignalMapper;
	delete mMouseMovementManager;
	delete mSelectList;
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
		if (items().contains(item) && !(item == mTopLeftCorner || item == mBottomRightCorner)) {
			removeItem(item);
		}
	}
}

void EditorViewScene::itemSelectUpdate()
{
	foreach (QGraphicsItem* item, *mSelectList) {
		item->setSelected(true);
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
				// check if we can add element into found parent
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
	foreach (qReal::Id type, mWindow->manager()->containedTypes(container.type())){
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

	QStringList targets;
	QStringList const groups = mWindow->manager()->paletteGroups(node->id(), node->id());

	foreach(PossibleEdge const &pEdge, edge->getPossibleEdges()) {
		// if pEdge.first.first is parent of node->id(), then add all children of pEdge.first.second to the list
		// and vice versa
		if (mWindow->manager()->isParentOf(node->id(), pEdge.first.first)) {
			targets << mWindow->manager()->allChildrenTypesOf(pEdge.first.second);
		}
		if (mWindow->manager()->isParentOf(node->id(), pEdge.first.second)) {
			targets << mWindow->manager()->allChildrenTypesOf(pEdge.first.first);
		}
	}

	QSet<QString> const targetsSet = targets.toSet();
	QMap<QString, QString> targetsInGroups;
	QStringList targetGroups;
	bool const chaoticEdition = SettingsManager::value("ChaoticEdition").toBool();
	foreach (QString const &group, groups) {
		QStringList const groupsContents = mWindow->manager()->paletteGroupList(
				node->id(), node->id(), group);
		foreach (QString const &elementInGroup, groupsContents) {
			if (targetsSet.contains(elementInGroup)) {
				targetsInGroups.insertMulti(group, elementInGroup);
				if (!targetGroups.contains(group) || chaoticEdition) {
					targetGroups.append(group);
				}
			}
		}
	}

	for (int i = 0; i < targetGroups.count(); ++i) {
		if (i > 0) {
			createElemMenu->addSeparator();
		}
		QStringList const targetsInGroup = targetsInGroups.values(targetGroups[i]);
		foreach (QString const &target, targetsInGroup) {
			Id const id = Id::loadFromString("qrm:/" + node->id().editor() + "/" + node->id().diagram() + "/" + target);
			QString const friendlyName = chaoticEdition ? "" : mWindow->manager()->friendlyName(id);
			QAction *element = new QAction(friendlyName, createElemMenu);
			// deleted as child of createElemMenu
			createElemMenu->addAction(element);
			QObject::connect(element,SIGNAL(triggered()), menuSignalMapper, SLOT(map()));
			menuSignalMapper->setMapping(element, id.toString());
		}
	}

	mCreatePoint = scenePos;
	mLastCreatedWithEdge = NULL;
	QObject::connect(menuSignalMapper, SIGNAL(mapped(QString const &)), this, SLOT(createElement(QString const &)));

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

qReal::Id EditorViewScene::createElement(QString const &str)
{
	qReal::Id result = createElement(str, mCreatePoint);
	mLastCreatedWithEdge = getElem(result);
	return result;
}

qReal::Id EditorViewScene::createElement(QString const &str, QPointF const &scenePos, bool searchForParents)
{
	Id typeId = Id::loadFromString(str);
	Id objectId(typeId.editor(),typeId.diagram(),typeId.element(),QUuid::createUuid().toString());

	QByteArray data;
	QMimeData *mimeData = new QMimeData();
	QDataStream stream(&data, QIODevice::WriteOnly);
	QString mimeType = QString("application/x-real-uml-data");
	QString uuid = objectId.toString();
	QString pathToItem = Id::rootId().toString();
	QString name = mWindow->manager()->friendlyName(typeId);
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

	Id id = Id::loadFromString(uuid);

	if(mMVIface->graphicalAssistApi()->editorManagerInter()->getPatternNames().contains(id.element())) {
		createGroupOfElements(id, scenePos, mMVIface->rootId(), isFromLogicalModel);
	} else {
		Element *newParent = NULL;
		Element *e = NULL;

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

			if(newParent && dynamic_cast<NodeElement*>(newParent)) {
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
		QPointF const position = !newParent ? scenePos
				: newParent->mapToItem(newParent, newParent->mapFromScene(scenePos));

		Id parentId = newParent ? newParent->id() : mMVIface->rootId();

		createSingleElement(id, name, e, position, parentId, isFromLogicalModel);

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
	}

	emit elementCreated(id);
}

void EditorViewScene::createSingleElement(Id const &id, QString const &name, Element * e
		, QPointF const &position, Id const &parentId, bool isFromLogicalModel)
{
	QList<NodeElement*> elements;
	Id newElemId = mMVIface->graphicalAssistApi()-> createElement(parentId, id, isFromLogicalModel , name, position);
	if (dynamic_cast<NodeElement*>(e)) {
		QSize size = mMVIface->graphicalAssistApi()->editorManagerInter()->iconSize(newElemId);
		getNodeById(newElemId)->setPos(position.x()- size.width()/2, position.y());
		elements.append(getNodeById(newElemId));
		insertElementIntoEdge(newElemId, newElemId, parentId, isFromLogicalModel, position
				, QPointF(size.width(), size.height()), elements);
	}
}

void EditorViewScene::createGroupOfElements(qReal::Id const &id, QPointF const &position, qReal::Id const&parentId
		, bool isFromLogicalModel)
{
	Pattern const pattern = mMVIface->graphicalAssistApi()->editorManagerInter()->getPatternByName(id.element());
	QList<NodeElement*> elements;
	QMap<QString, Id> nodes;
	QPointF size = pattern.size();
	foreach (GroupNode const &node, pattern.nodes()) {
		Id const element(id.editor(), id.diagram(), node.type, QUuid::createUuid().toString());
		Id newElemId = mMVIface->graphicalAssistApi()->createElement(parentId
				, element, isFromLogicalModel, "(" + node.type + ")", position);
		getNodeById(newElemId)->setPos(position.x()- size.x()/2 + node.position.x()
				, position.y() + node.position.y());
		nodes.insert(node.id, newElemId);
		elements.append(getNodeById(newElemId));
	}

	foreach (GroupEdge const &edge, pattern.edges()) {
		Id const element(id.editor(), id.diagram(), edge.type, QUuid::createUuid().toString());
		mMVIface->graphicalAssistApi()-> createElement(parentId, element, isFromLogicalModel
				, "(" + edge.type + ")", QPointF(0,0));
		mMVIface->graphicalAssistApi()-> setFrom(element, nodes.value(edge.from));
		mMVIface->graphicalAssistApi()-> setTo(element, nodes.value(edge.to));
		getNodeById(nodes.value(edge.to))->connectLinksToPorts();
		reConnectLink(getEdgeById(element));
	}
	insertElementIntoEdge(nodes.value(pattern.inNode()), nodes.value(pattern.outNode())
			, parentId, isFromLogicalModel, position, size, elements);
}

void EditorViewScene::insertElementIntoEdge(qReal::Id const &insertedFirstNodeId
		, qReal::Id const &insertedLastNodeId , qReal::Id const &parentId, bool isFromLogicalModel
		, QPointF const &scenePos, QPointF const &shift, QList<NodeElement*> elements)
{
	foreach (QGraphicsItem *item, items(scenePos)) {
		EdgeElement *edge = dynamic_cast<EdgeElement*>(item);
		if(edge && edge->isDividable()) {// check if item is an edge and the edge is dissectable
			NodeElement *previouslyConnectedFrom = edge->src();
			NodeElement *previouslyConnectedTo = edge->dst();
			if (previouslyConnectedTo && previouslyConnectedFrom) {//check has edge dst
				Id realParentId = (parentId == Id::rootId()) ? mMVIface->rootId() : parentId;

				Id const newEdge1(edge->id().editor(), edge->id().diagram()
								  , edge->id().element(), QUuid::createUuid().toString());
				mMVIface->graphicalAssistApi()-> createElement(realParentId, newEdge1
															   , isFromLogicalModel, "flow1", scenePos);
				mMVIface->graphicalAssistApi()->setFrom(newEdge1, previouslyConnectedFrom->id());
				mMVIface->graphicalAssistApi()->setTo(newEdge1, insertedFirstNodeId);
				getNodeById(insertedFirstNodeId)->connectLinksToPorts();

				reConnectLink(getEdgeById(newEdge1));

				Id const newEdge2(edge->id().editor(), edge->id().diagram()
								  , edge->id().element(), QUuid::createUuid().toString());
				mMVIface->graphicalAssistApi()-> createElement(realParentId, newEdge2
															   , isFromLogicalModel, "flow2", scenePos);
				mMVIface->graphicalAssistApi()->setFrom(newEdge2, insertedLastNodeId);
				mMVIface->graphicalAssistApi()->setTo(newEdge2, previouslyConnectedTo->id());
				previouslyConnectedTo->connectLinksToPorts();

				reConnectLink(getEdgeById(newEdge2));

				QPointF fromP = previouslyConnectedFrom->pos();
				QPointF toP = previouslyConnectedTo->pos();
				QPointF direction = QPointF(toP.x()- fromP.x(), toP.y()- fromP.y());
				mainWindow()->deleteElementFromDiagram(edge->id());

				moveDownFromElem(getNodeById(insertedLastNodeId), scenePos, direction, shift, elements);
				moveDownFromElem(getNodeById(insertedFirstNodeId), scenePos, direction, shift, elements);

				break;
			}
		}
	}
}

void EditorViewScene::moveDownFromElem(NodeElement* node, QPointF const &scenePos
		, QPointF const &direction, QPointF const &shift, QList<NodeElement*> moved)
{
	if (direction.x() == 0 && direction.y() == 0) {
		return;
	}
	QList<NodeElement*> destinations = getNeibors(node);
	for (int i = 0; i < destinations.length(); i++) {
		if (!moved.contains(destinations.at(i))) {
			if (destinations.at(i)->pos().y() >= scenePos.y()) {
				if(direction.x() == 0 || shift.y() < shift.x() * sign(direction.x()) * sign(direction.y())
						* (direction.y() / direction.x())
				) {
					destinations.at(i)->setPos(destinations.at(i)->pos().x() + shift.y() * direction.x() / direction.y()
							, destinations.at(i)->pos().y() + shift.y());
				} else {
					destinations.at(i)->setPos(destinations.at(i)->pos().x() + sign(direction.x()) * sign(direction.y())* shift.x()
							, destinations.at(i)->pos().y() + shift.x() * sign(direction.x()) * sign(direction.y())
									* direction.y() / direction.x());
				}
				arrangeNodeLinks(destinations.at(i));
			}
			moved.append(destinations.at(i));
			moveDownFromElem(destinations.at(i), scenePos, direction, shift, moved);
		}
	}
}//

void EditorViewScene::deleteElementFromEdge(qReal::Id const &nodeId, QList<QGraphicsItem *> edgesToDelete)
{
	QList<EdgeElement*> inEdges = getInEdges(getNodeById(nodeId));
	QList<EdgeElement*> outEdges = getOutEdges(getNodeById(nodeId));
	if (inEdges.count() == 1 && !edgesToDelete.contains(dynamic_cast<QGraphicsItem*>(inEdges.at(0)))
			&& outEdges.count() > 0)
	{
		NodeElement* node = inEdges.at(0)->src();
		bool wasRelinked = false;
		if (node) {
			foreach(EdgeElement* edge, outEdges) {
				if (!edgesToDelete.contains(dynamic_cast<QGraphicsItem*>(edge))) {
					mMVIface->graphicalAssistApi()->setFrom(edge->id(), node->id());
					getNodeById(edge->dst()->id())->connectLinksToPorts();
					reConnectLink(edge);
					wasRelinked = true;
				}
			}
			if (wasRelinked) {
				mainWindow()->deleteElementFromDiagram(inEdges.at(0)->id());
			}
		}
		return;
	}
	if (outEdges.count() == 1 && !edgesToDelete.contains(dynamic_cast<QGraphicsItem*>(outEdges.at(0)))
			&& inEdges.count() > 0)
	{
		NodeElement* node = outEdges.at(0)->dst();
		if (node) {
			bool wasRelinked = false;
			foreach(EdgeElement* edge, inEdges) {
				if(!edgesToDelete.contains(dynamic_cast<QGraphicsItem*>(edge))) {
					mMVIface->graphicalAssistApi()->setTo(edge->id(), node->id());
					getNodeById(node->id())->connectLinksToPorts();
					reConnectLink(edge);
					wasRelinked = true;
				}
			}
			if (wasRelinked) {
				mainWindow()->deleteElementFromDiagram(outEdges.at(0)->id());
			}
		}
	}
}

qreal EditorViewScene::sign(qreal x)
{
	return (x >= 0) ? 1 : -1;
}

void EditorViewScene::reConnectLink(EdgeElement * edgeElem)
{
	if (edgeElem->src()) {
		arrangeNodeLinks(edgeElem->src());
	}
	if (edgeElem->dst()) {
		arrangeNodeLinks(edgeElem->dst());
	}
}

void EditorViewScene::arrangeNodeLinks(NodeElement* node)
{
	node->arrangeLinks();
	foreach (EdgeElement* nodeEdge, node->edgeList()) {
		nodeEdge->adjustNeighborLinks();
		nodeEdge->correctArrow();
		nodeEdge->correctInception();
		nodeEdge->setGraphicApiPos();
		nodeEdge->saveConfiguration(QPointF());
	}
	node->arrangeLinks();
	node->adjustLinks();
}

NodeElement* EditorViewScene::getNodeById(qReal::Id const &itemId)
{
	foreach (QGraphicsItem *item, items()) {
		NodeElement *node = dynamic_cast<NodeElement*>(item);
		if (node && node->id() == itemId) {
			return node;
		}
	}
	return NULL;
}

EdgeElement* EditorViewScene::getEdgeById(qReal::Id const &itemId)
{
	foreach (QGraphicsItem *item, items()) {
		EdgeElement *edge = dynamic_cast<EdgeElement*>(item);
		if (edge && edge->id() == itemId) {
			return edge;
		}
	}
	return NULL;
}

QList<EdgeElement*> EditorViewScene::getInEdges(NodeElement* node)
{
	QList<EdgeElement*> edges;
	foreach (EdgeElement* edge, node->getEdges()) {
		if (edge->dst() == node)
			edges.append(edge);
	}
	return edges;
}

QList<EdgeElement*> EditorViewScene::getOutEdges(NodeElement* node)
{
	QList<EdgeElement*> edges;
	foreach (EdgeElement* edge, node->getEdges()) {
		if (edge->src() == node)
			edges.append(edge);
	}
	return edges;
}

QList<NodeElement*> EditorViewScene::getNeibors(NodeElement *node)
{
	QList<NodeElement*> list;
	foreach (EdgeElement *edge, node->getEdges()) {
		if (edge->src()->id() == node->id()) {
			list.append(edge->dst());
		}
		if (edge->dst()->id() == node->id()) {
			list.append(edge->src());
		}
	}
	return list;
}

void EditorViewScene::copy()
{
	QList<NodeElement *> nodes = getNodesForCopying();

	QList<NodeData> nodesData = getNodesData(nodes);
	QList<EdgeData> edgesData = getEdgesData(nodes);

	pushDataToClipboard(nodesData, edgesData);
}

QList<NodeData> EditorViewScene::getNodesData(QList<NodeElement *> const &nodes)
{
	QList<NodeData> nodesData;
	foreach (NodeElement* node, nodes) {
		nodesData << node->data();
	}
	return nodesData;
}

QList<NodeElement *> EditorViewScene::getNodesForCopying()
{
	QList<NodeElement *> nodes;
	foreach (QGraphicsItem *item, selectedItems()) {
		NodeElement *node = dynamic_cast<NodeElement *>(item);
		if (node && !selectedItems().contains(node->parentItem())) {
			nodes << node;
		}
	}
	foreach (NodeElement *node, nodes) {
		addChildren(node, nodes);
	}

	return nodes;
}

void EditorViewScene::addChildren(NodeElement *node, QList<NodeElement *> &nodes)
{
	foreach (QGraphicsItem *item, node->childItems()) {
		NodeElement *child = dynamic_cast<NodeElement *>(item);
		if (child && !nodes.contains(child)) {
			nodes << child;
			addChildren(child, nodes);
		}
	}
}

QList<EdgeData> EditorViewScene::getEdgesData(QList<NodeElement *> const &nodes)
{
	QList<EdgeData> edgesData;
	foreach (NodeElement* node, nodes) {
		foreach (EdgeElement *edge, node->getEdges()) {
			EdgeData& data = edge->data();
			if (nodes.contains(edge->src()) && nodes.contains(edge->dst())
					&& !edgesData.contains(data)) {
				edgesData << data;
			}
		}
	}
	return edgesData;
}

void EditorViewScene::pushDataToClipboard(QList<NodeData> const &nodesData, QList<EdgeData> const &edgesData)
{
	QByteArray data;
	QDataStream stream(&data, QIODevice::WriteOnly);

	stream << nodesData;
	stream << edgesData;

	QMimeData *mimeData = new QMimeData();
	mimeData->setData("application/x-real-uml-model-data", data);

	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setMimeData(mimeData);
}

void EditorViewScene::paste(bool isGraphicalCopy)
{
	QList<NodeData> nodesData;
	QList<EdgeData> edgesData;
	pullDataFromClipboard(nodesData, edgesData);

	if (nodesData.isEmpty()) {
		return;
	}

	QPointF offset = getMousePos() - nodesData[0].pos;

	QHash<Id, Id> copiedIds = pasteNodes(nodesData, offset, isGraphicalCopy);

	foreach (EdgeData data, edgesData) {
		pasteEdge(data, isGraphicalCopy, copiedIds, offset);
	}
}

void EditorViewScene::pullDataFromClipboard(QList<NodeData> &nodesData, QList<EdgeData> &edgesData)
{
	QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();

	QByteArray data = mimeData->data("application/x-real-uml-model-data");
	QDataStream stream(&data, QIODevice::ReadOnly);

	stream >> nodesData;
	stream >> edgesData;
}

QHash<Id, Id> EditorViewScene::pasteNodes(QList<NodeData> &nodesData
		, QPointF const &offset, bool isGraphicalCopy)
{
	QHash<Id, Id> copiedIds;

	while (!nodesData.isEmpty()) {
		NodeData& nextToPaste = nodesData[0];
		Id copy = pasteNode(nextToPaste, isGraphicalCopy, copiedIds, offset);
		copiedIds.insert(nextToPaste.id, copy);
		nodesData.removeAll(nextToPaste);
	}

	return copiedIds;
}

Id EditorViewScene::pasteNode(NodeData const &nodeData, bool isGraphicalCopy
		, QHash<Id, Id> const &copiedIds, QPointF const &offset)
{
	QPointF newPos = getNewPos(nodeData, copiedIds, offset);
	NodeElement *newNode = NULL;

	if (isGraphicalCopy) {
		newNode = pasteGraphicalCopyOfNode(nodeData, newPos);
	} else {
		newNode = pasteNewNode(nodeData, newPos);
	}

	restoreNode(newNode, nodeData, copiedIds, newPos);

	return newNode->id();
}

NodeElement *EditorViewScene::pasteGraphicalCopyOfNode(NodeData const &nodeData
		, QPointF const &newPos)
{
	QString name = mMVIface->graphicalAssistApi()->name(nodeData.id);

	Id newId = mMVIface->graphicalAssistApi()->createElement(rootItemId(), nodeData.logicalId, true, name, newPos);
	NodeElement *newNode = dynamic_cast<NodeElement *>(mainWindow()->manager()->graphicalObject(newId));

	newNode->setAssistApi(mMVIface->graphicalAssistApi(), mMVIface->logicalAssistApi());
	newNode->setId(newId);

	return newNode;
}

NodeElement *EditorViewScene::pasteNewNode(NodeData const &data, QPointF const &newPos)
{
	Id typeId = data.id.type();
	Id newId = createElement(typeId.toString(), newPos);
	NodeElement *newNode = dynamic_cast<NodeElement *>(getElem(newId));

	return newNode;
}

QPointF EditorViewScene::getNewPos(NodeData const &nodeData
		, QHash<Id, Id> const &copiedIds, QPointF const &offset)
{
	if (!copiedIds.contains(nodeData.parentId)) {
		return nodeData.pos + offset;
	}
	return nodeData.pos;
}

void EditorViewScene::restoreNode(NodeElement *node, NodeData const &nodeData
		, QHash<Id, Id> const &copiedIdsMap, QPointF const &pos)
{
	Id nodeId = node->id();

	mMVIface->graphicalAssistApi()->setProperties(nodeId, nodeData.properties);

	if (copiedIdsMap.contains(nodeData.parentId)) {
		mMVIface->graphicalAssistApi()->changeParent(nodeId, copiedIdsMap[nodeData.parentId], pos);
	}
}

Id EditorViewScene::pasteEdge(EdgeData const &edgeData, bool isGraphicalCopy
		, QHash<Id, Id> const &copiedIds, QPointF const &offset)
{
	EdgeElement *newEdge;

	if (isGraphicalCopy) {
		newEdge = pasteGraphicalCopyOfEdge(edgeData);
	} else {
		newEdge = pasteNewEdge(edgeData);
	}

	restoreEdge(newEdge, edgeData, copiedIds, edgeData.pos + offset);
	return newEdge->id();
}

EdgeElement *EditorViewScene::pasteGraphicalCopyOfEdge(EdgeData const &edgeData)
{
	QString name = mMVIface->graphicalAssistApi()->name(edgeData.id);

	Id newId = mMVIface->graphicalAssistApi()->createElement(
			rootItemId(), edgeData.logicalId, true, name, edgeData.pos);

	EdgeElement *newEdge = dynamic_cast<EdgeElement *>(mainWindow()->manager()->graphicalObject(newId));
	newEdge->setAssistApi(mMVIface->graphicalAssistApi(), mMVIface->logicalAssistApi());
	newEdge->setId(newId);

	return newEdge;
}

EdgeElement *EditorViewScene::pasteNewEdge(EdgeData const &edgeData)
{
	Id typeId = edgeData.id.type();
	Id newId = createElement(typeId.toString(), QPointF());

	EdgeElement *newEdge = dynamic_cast<EdgeElement *>(getElem(newId));
	newEdge->connectToPort();

	return newEdge;
}

void EditorViewScene::restoreEdge(EdgeElement *edge, EdgeData const &edgeData
		, QHash<Id, Id> const &copiedIdsMap, QPointF const &pos)
{
	Id edgeId = edge->id();

	Id newSrcId = copiedIdsMap[edgeData.srcId];
	Id newDstId = copiedIdsMap[edgeData.dstId];

	mMVIface->graphicalAssistApi()->setPosition(edgeId, pos);
	mMVIface->graphicalAssistApi()->setConfiguration(edgeId, edgeData.configuration);

	mMVIface->graphicalAssistApi()->setFrom(edgeId, newSrcId);
	mMVIface->graphicalAssistApi()->setTo(edgeId, newDstId);

	mMVIface->graphicalAssistApi()->setFromPort(edgeId, edgeData.portFrom);
	mMVIface->graphicalAssistApi()->setToPort(edgeId, edgeData.portTo);
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
		paste(event->modifiers() == Qt::ShiftModifier);
	} else if (event->matches(QKeySequence::Copy)) {
		copy();
	} else if (isArrow(event->key())) {
		moveSelectedItems(event->key());
	} else if (event->key() == Qt::Key_Menu) {
		initContextMenu(NULL, QPointF()); // see #593
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

inline bool EditorViewScene::isArrow(int key)
{
	return key == Qt::Key_Left || key == Qt::Key_Right || key == Qt::Key_Down || key == Qt::Key_Up;
}

void EditorViewScene::moveSelectedItems(int direction)
{
	mOffset = offsetByDirection(direction);
	if (mOffset == QPointF(0, 0)) {
		return;
	}

	foreach (QGraphicsItem* item, selectedItems()) {
		QPointF newPos = item->pos();
		newPos += mOffset;

		Element* element = dynamic_cast<Element*>(item);
		if (element) {
			element->setPos(newPos);
		}

		NodeElement* node = dynamic_cast<NodeElement*>(item);
		if (node) {
			node->alignToGrid();
			node->adjustLinks(true);
		} else {
			EdgeElement* edge = dynamic_cast<EdgeElement*>(item);
			if (edge && !(edge->src() && edge->dst()) && (edge->src() || edge->dst())
					&& (edge->src() ? !edge->src()->isSelected() : true)
					&& (edge->dst() ? !edge->dst()->isSelected() : true)) {
				edge->adjustLink();
			}
		}
	}

	mTimerForArrowButtons->start(700);
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
	bool hasAnyActions = false;
	QMenu *addConnectionMenu = new QMenu(menuName);

	foreach (Id type, connectableTypes) {
		foreach (Id elementId, mMVIface->logicalAssistApi()->logicalRepoApi().logicalElements(type)) {
			if (alreadyConnectedElements.contains(elementId)) {
				continue;
			}
			QAction *action = addConnectionMenu->addAction(mMVIface->logicalAssistApi()->logicalRepoApi().name(elementId));
			hasAnyActions = true;
			connect(action, SIGNAL(triggered()), slot);
			QList<QVariant> tag;
			tag << element->logicalId().toVariant() << elementId.toVariant();
			action->setData(tag);
		}
	}

	foreach (Id diagram, connectableDiagrams) {
		Id const diagramType = mMVIface->logicalAssistApi()->editorManagerInter()->findElementByType(diagram.element());
		QString const name = mMVIface->logicalAssistApi()->editorManagerInter()->friendlyName(diagramType);
		QString const editorName = mMVIface->logicalAssistApi()->editorManagerInter()->friendlyName(Id(diagramType.editor()));
		QAction *action = addConnectionMenu->addAction("New " + editorName + "/" + name);
		hasAnyActions = true;
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->logicalId().toVariant() << diagramType.toVariant();
		action->setData(tag);
	}
	if (hasAnyActions || !connectableDiagrams.empty())
	{
		contextMenu.addMenu(addConnectionMenu);
	}
}

void EditorViewScene::createDisconnectMenu(Element const * const element, QMenu &contextMenu, QString const &menuName
		, IdList const &outgoingConnections, IdList const &incomingConnections, const char *slot) const
{
	QMenu *disconnectMenu = new QMenu(menuName);//contextMenu.addMenu(menuName);
	IdList list = outgoingConnections;
	list.append(incomingConnections);

	foreach (Id elementId, list) {
		QAction *action = disconnectMenu->addAction(mMVIface->logicalAssistApi()->logicalRepoApi().name(elementId));
		connect(action, SIGNAL(triggered()), slot);
		QList<QVariant> tag;
		tag << element->logicalId().toVariant() << elementId.toVariant();
		action->setData(tag);
	}
	if (!list.empty()) {
		contextMenu.addMenu(disconnectMenu);
	}
}

void EditorViewScene::createConnectionSubmenus(QMenu &contextMenu, Element const * const element) const
{
	if (mainWindow()->showConnectionRelatedMenus()) {
		// menu items "connect to"
		// TODO: move to elements, they can call the model and API themselves
		createAddConnectionMenu(element, contextMenu, tr("Add connection")
				, mWindow->manager()->connectedTypes(element->id().type())
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId())
				, mMVIface->logicalAssistApi()->diagramsAbleToBeConnectedTo(element->logicalId())
				, SLOT(connectActionTriggered()));

		createDisconnectMenu(element, contextMenu, tr("Disconnect")
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId())
				, mMVIface->logicalAssistApi()->logicalRepoApi().incomingConnections(element->logicalId())
				, SLOT(disconnectActionTriggered()));

		createAddConnectionMenu(element, contextMenu, tr("Add usage")
				, mWindow->manager()->usedTypes(element->id().type())
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId())
				, mMVIface->logicalAssistApi()->diagramsAbleToBeUsedIn(element->logicalId())
				, SLOT(addUsageActionTriggered()));

		createDisconnectMenu(element, contextMenu, tr("Delete usage")
				, mMVIface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId())
				, mMVIface->logicalAssistApi()->logicalRepoApi().incomingUsages(element->logicalId())
				, SLOT(deleteUsageActionTriggered()));

		QMenu * const goToMenu = contextMenu.addMenu(tr("Go to"));

		createGoToSubmenu(goToMenu, tr("Forward connection"), mMVIface->logicalAssistApi()->logicalRepoApi().outgoingConnections(element->logicalId()));
		createGoToSubmenu(goToMenu, tr("Backward connection"), mMVIface->logicalAssistApi()->logicalRepoApi().incomingConnections(element->logicalId()));
		createGoToSubmenu(goToMenu, tr("Uses"), mMVIface->logicalAssistApi()->logicalRepoApi().outgoingUsages(element->logicalId()));
		createGoToSubmenu(goToMenu, tr("Used in"), mMVIface->logicalAssistApi()->logicalRepoApi().incomingUsages(element->logicalId()));
	}
	if (mWindow->manager()->isInterpretationMode()) {
		contextMenu.addSeparator();
		QAction * const changePropertiesAction = contextMenu.addAction(tr("Change Properties"));
		connect(changePropertiesAction, SIGNAL(triggered()), SLOT(changePropertiesActionTriggered()));
		changePropertiesAction->setData(element->id().toVariant());
		QAction * const changeAppearancePaletteAction = contextMenu.addAction(tr("Change Appearance"));
		connect(changeAppearancePaletteAction, SIGNAL(triggered()), SLOT(changeAppearanceActionTriggered()));
		changeAppearancePaletteAction->setData(element->id().toVariant());
	}
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	mCurrentMousePos = event->scenePos();
	if (mLeftButtonPressed && !mIsSelectEvent && selectedItems().size() == 1) {
		if (event->button() == Qt::RightButton) {
			QGraphicsItem *item = mouseGrabberItem();
			EdgeElement *edge = dynamic_cast <EdgeElement *> (item);
			if (edge) {
				sendEvent(edge, event);
			}
		}
		return;
	}

	// Let scene update selection and perform other operations
	QGraphicsItem* item = itemAt(mCurrentMousePos, QTransform());
	if (event->modifiers() & Qt::ControlModifier) {
		if (item) {
			QGraphicsScene::mousePressEvent(event);
		}
	} else {
		QGraphicsScene::mousePressEvent(event);
	}

	if ((event->modifiers() & Qt::ControlModifier) && (event->buttons() & Qt::LeftButton) && !(event->buttons() & Qt::RightButton)) {
		mIsSelectEvent = true;
		mSelectList->append(selectedItems());
		foreach (QGraphicsItem* item, items()) {
			item->setAcceptedMouseButtons(0);
		}
		foreach (QGraphicsItem* item, *mSelectList) {
			item->setSelected(true);
		}
		QGraphicsItem* item = itemAt(mCurrentMousePos, QTransform());
		if (item) {
			item->setSelected(!mSelectList->contains(item));
			if (item->isSelected()) {
				mSelectList->append(item);
			} else {
				mSelectList->removeAll(item);
			}
		}
	}

	if (event->button() == Qt::LeftButton) {
		mLeftButtonPressed = true;
		QGraphicsItem *item = itemAt(event->scenePos(), QTransform());
		ElementTitle *title = dynamic_cast < ElementTitle *>(item);

		if (title) { // check whether we accidently clicked on a title or not
			item = item->parentItem();
		}

		if (item) {
			mPrevParent = item->parentItem();
			mPrevPosition = item->pos();
		}

	} else {
		if (event->button() == Qt::RightButton && !(event->buttons() & Qt::LeftButton)) {
			mTimer->stop();
			mMouseMovementManager->mousePress(event->scenePos());
			mRightButtonPressed = true;
		}
	}
	redraw();

	mShouldReparentItems = (selectedItems().size() > 0);
}

void EditorViewScene::initContextMenu(Element *e, const QPointF &pos)
{
	QMenu menu;

	disableActions(e);
	menu.addActions(mContextMenuActions);

	if (e) {
		QList<ContextMenuAction*> elementActions = e->contextMenuActions(e->mapFromScene(pos));

		if (!elementActions.isEmpty()) {
			menu.addSeparator();
		}

		foreach (ContextMenuAction* action, elementActions) {
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

	enableActions();
}

void EditorViewScene::disableActions(Element *focusElement)
{
	if (!focusElement) {
		mWindow->actionDeleteFromDiagram()->setEnabled(false);
		mWindow->actionCopyElementsOnDiagram()->setEnabled(false);
	}
	if (isEmptyClipboard()) {
		mWindow->actionPasteOnDiagram()->setEnabled(false);
		mWindow->actionPasteCopyOfLogical()->setEnabled(false);
	}
}

void EditorViewScene::enableActions()
{
	mWindow->actionDeleteFromDiagram()->setEnabled(true);
	mWindow->actionCopyElementsOnDiagram()->setEnabled(true);
	mWindow->actionPasteOnDiagram()->setEnabled(true);
	mWindow->actionPasteCopyOfLogical()->setEnabled(true);
}

bool EditorViewScene::isEmptyClipboard()
{
	QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();
	return mimeData->data("application/x-real-uml-model-data").isEmpty();
}

void EditorViewScene::getObjectByGesture()
{
	mTimer->stop();
	qReal::Id id = mMouseMovementManager->getObject();
	if (!id.element().isEmpty()) {
		createElement(id.toString(), mMouseMovementManager->pos());
	}
	deleteGesture();
}

void EditorViewScene::updateMovedElements()
{
	mTimerForArrowButtons->stop();

	if (mOffset == QPointF(0, 0)) {
		return;
	}

	foreach (QGraphicsItem* item, selectedItems()) {
		QPointF newPos = item->pos();

		Element* element = dynamic_cast<Element*>(item);
		if (element) {
			mMVIface->graphicalAssistApi()->setPosition(element->id(), newPos);
		}
	}
}

void EditorViewScene::getLinkByGesture(NodeElement *parent, const NodeElement &child)
{
	QList<PossibleEdge> edges = parent->getPossibleEdges();
	QList<QString> allLinks;
	foreach (PossibleEdge const &possibleEdge, edges) {
		if (possibleEdge.first.second.editor() == child.id().editor()
		&& possibleEdge.first.second.diagram() == child.id().diagram()
		&& mainWindow()->manager()->isParentOf(child.id().editor(), child.id().diagram()
		, possibleEdge.first.second.element(), child.id().diagram(), child.id().element()))
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

	QObject::connect(menuSignalMapper, SIGNAL(mapped(QString const &)), this, SLOT(createEdge(QString)));
	edgeMenu->exec(QCursor::pos());
}

void EditorViewScene::createEdge(QString const &idStr)
{
	QPointF start = mMouseMovementManager->firstPoint();
	QPointF end = mMouseMovementManager->lastPoint();
	Id id = createElement(idStr, start);
	Element *edgeElement = getElem(id);
	EdgeElement *edge = dynamic_cast <EdgeElement *> (edgeElement);
	edge->setSrc(NULL);
	edge->setDst(NULL);

	edge->placeEndTo(edge->mapFromScene(end));
	edge->connectToPort();
	if (edge->dst()) {
		edge->dst()->arrangeLinks();
		foreach (EdgeElement* nodeEdge, edge->dst()->edgeList()) {
			nodeEdge->adjustNeighborLinks();
			nodeEdge->correctArrow();
			nodeEdge->correctInception();
			nodeEdge->setGraphicApiPos();
			nodeEdge->saveConfiguration(QPointF());
		}
		edge->dst()->arrangeLinks();
		edge->dst()->adjustLinks();
	}
}

void EditorViewScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if ((event->button() == Qt::LeftButton) && !(event->buttons() & Qt::RightButton)) {
		mLeftButtonPressed = false;
	}

	if (mIsSelectEvent && (event->button() == Qt::LeftButton)) {
		foreach (QGraphicsItem* item, items()) {
			item->setAcceptedMouseButtons(Qt::MouseButtons(Qt::RightButton | Qt::LeftButton));
		}
		mIsSelectEvent = false;
		foreach (QGraphicsItem* item, *mSelectList) {
			item->setSelected(true);
		}
		mSelectList->clear();
		return;
	}

	if (mLeftButtonPressed && !mIsSelectEvent) {
		return;
	}

	if (!(mLeftButtonPressed && event->button() == Qt::RightButton)) {
		QGraphicsScene::mouseReleaseEvent(event);
	}

	Element *element = getElemAt(event->scenePos());

	if (mShouldReparentItems) {
		QList<QGraphicsItem *> const list = selectedItems();
		foreach(QGraphicsItem *item, list) {
			EdgeElement* edgeItem = dynamic_cast<EdgeElement*>(item);
			if (edgeItem) {
				if (!list.contains(edgeItem->src()) && !list.contains(edgeItem->dst()) && (edgeItem->src() || edgeItem->dst())) {
					edgeItem->arrangeAndAdjustHandler(QPointF());
				}
			} else {
				NodeElement* nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem) {
					Element *e = dynamic_cast<Element *>(itemAt(event->scenePos(), QTransform()));
					if ((e && (nodeItem->id() != e->id())) || !e) {
						sendEvent(item, event);
					}
					if (list.size() > 1 && nodeItem) {
						nodeItem->setVisibleEmbeddedLinkers(false);
						nodeItem->setPortsVisible(false);
					}
				}
			}
		}
		// in case there'll be 2 consecutive release events
		mShouldReparentItems = false;
	}

	if (event->button() == Qt::RightButton && !(mMouseMovementManager->pathIsEmpty())) {
		mMouseMovementManager->mouseMove(event->scenePos());
		mRightButtonPressed = false;
		drawGesture();
		EdgeElement * const edgeElement = dynamic_cast<EdgeElement *>(element);
		if (edgeElement) {
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
		if (parent && child && mMouseMovementManager->isEdgeCandidate()
				&& parent->id() != child->id()) {
			getLinkByGesture(parent, *child);
			deleteGesture();
		} else {
			mTimer->start(SettingsManager::value("gestureDelay").toInt());
		}
		return;
	}

	if (element) {
		redraw();
	}
}

void EditorViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	mCurrentMousePos = event->scenePos();
	if ((mLeftButtonPressed && !(event->buttons() & Qt::RightButton))) {
		QGraphicsScene::mouseMoveEvent(event);
	} else {
		// button isn't recognized while mouse moves
		if (mRightButtonPressed) {
			mMouseMovementManager->mouseMove(event->scenePos());
			drawGesture();
		} else {
			QGraphicsScene::mouseMoveEvent(event);
		}
	}
}

QPointF EditorViewScene::getMousePos()
{
	return mCurrentMousePos;
}

void EditorViewScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && !event->modifiers()) {
		// Double click on a title activates it
		if (ElementTitle *title = dynamic_cast<ElementTitle*>(itemAt(event->scenePos(), QTransform()))) {
			if (!title->hasFocus()) {  // Do not activate already activated item
				event->accept();
				title->startTextInteraction();
				return;
			}
		}
		else if (NodeElement *element = dynamic_cast<NodeElement*>(itemAt(event->scenePos(), QTransform()))) {
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
					Id diagramType = mMVIface->logicalAssistApi()->editorManagerInter()->findElementByType(diagrams[0].element());
					mMVIface->logicalAssistApi()->createConnected(element->logicalId(), diagramType);
				}
			}
		}
	}

	Element *e = dynamic_cast<Element *>(itemAt(event->scenePos(), QTransform()));
	if (e && e->isSelected() && !event->modifiers()) {
		mainWindow()->graphicalModelExplorer()->setFocus();
		mView->ensureElementVisible(e);
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
	mContextMenuActions << mWindow->actionDeleteFromDiagram()
			<< mWindow->actionCopyElementsOnDiagram()
			<< mWindow->actionPasteOnDiagram() << mWindow->actionPasteCopyOfLogical();
	// TODO: what is it?
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

void EditorViewScene::changePropertiesActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id id = action->data().value<Id>();
	PropertiesDialog *propDialog = new PropertiesDialog(mWindow);
	propDialog->init(mWindow->manager(), id);
	propDialog->setModal(true);
	propDialog->show();
}

void EditorViewScene::changeAppearanceActionTriggered()
{
	QAction *action = static_cast<QAction *>(sender());
	Id id = action->data().value<Id>();
	QString propertyValue = mWindow->manager()->shape(id);
	mWindow->openShapeEditor(id, propertyValue, mWindow->manager());
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
	if (wheelEvent->modifiers() == Qt::ControlModifier) {
		if (wheelEvent->delta() > 0) {
			emit zoomIn();
		} else {
			emit zoomOut();
		}
		wheelEvent->accept();
	}
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
	if (!mLeftButtonPressed) {
		foreach (QGraphicsItem *element, items()) {
			element->setSelected(true);
		}
	}
}

void EditorViewScene::initCorners()
{
	mTopLeftCorner->setVisible(false);
	mBottomRightCorner->setVisible(false);

	setCorners(QPointF(0, 0), QPointF(1000, 1000));
}

void EditorViewScene::setCorners(QPointF const &topLeft, QPointF const &bottomRight)
{
	mTopLeftCorner->setPos(topLeft);
	mBottomRightCorner->setPos(bottomRight);

	addItem(mTopLeftCorner);
	addItem(mBottomRightCorner);
}

// this needs to do something with it: make the behavior of sceneRect adequate
void EditorViewScene::cropToItems()
{
	removeItem(mTopLeftCorner);
	removeItem(mBottomRightCorner);

	QRectF newRect = itemsBoundingRect();

	setSceneRect(newRect);
	mView->setSceneRect(newRect);
	setCorners(newRect.topLeft(), newRect.bottomRight());
}

void EditorViewScene::updateEdgeElements()
{
	foreach (QGraphicsItem *item, items()) {
		EdgeElement* element = dynamic_cast<EdgeElement*>(item);
		if (element) {
			element->redrawing(QPoint());
		}
	}
}

void EditorViewScene::updateEdgesViaNodes()
{
	foreach (QGraphicsItem *item, items()) {
		NodeElement* node = dynamic_cast<NodeElement*>(item);
		if (node) {
			foreach (EdgeElement* edge, node->edgeList()) {
				edge->correctArrow();
				edge->correctInception();
			}
			node->adjustLinks();
		}
	}
}

void EditorViewScene::setTitlesVisible(bool visible)
{
	mTitlesVisible = visible;
	foreach (QGraphicsItem *item, items()) {
		Element *element = dynamic_cast<Element *>(item);
		if (element) {
			element->setTitlesVisible(visible);
		}
	}
}

void EditorViewScene::onElementParentChanged(Element *element)
{
	element->setTitlesVisible(mTitlesVisible);
}
