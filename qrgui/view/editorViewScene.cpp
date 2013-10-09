﻿#include "editorViewScene.h"

#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <math.h>

#include "view/editorView.h"
#include "mainwindow/mainWindow.h"
#include "dialogs/metamodelingOnFly/propertiesDialog.h"

#include "controller/commands/createElementCommand.h"
#include "controller/commands/createGroupCommand.h"
#include "umllib/private/reshapeEdgeCommand.h"
#include "umllib/private/resizeCommand.h"
#include "controller/commands/insertIntoEdgeCommand.h"
#include "umllib/private/expandCommand.h"

using namespace qReal;
using namespace qReal::commands;
using namespace qReal::gui;

EditorViewScene::EditorViewScene(QObject *parent)
		: QGraphicsScene(parent)
		, mLastCreatedWithEdge(NULL)
		, mClipboardHandler(this)
		, mRightButtonPressed(false)
		, mLeftButtonPressed(false)
		, mHighlightNode(NULL)
		, mWindow(NULL)
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
		, mExploser(NULL)
{
	mNeedDrawGrid = SettingsManager::value("ShowGrid").toBool();
	mWidthOfGrid = static_cast<double>(SettingsManager::value("GridWidth").toInt()) / 100;
	mRealIndexGrid = SettingsManager::value("IndexGrid").toInt();

	setItemIndexMethod(NoIndex);
	setEnabled(false);

	initCorners();

	connect(mTimer, SIGNAL(timeout()), this, SLOT(getObjectByGesture()));
	connect(mTimerForArrowButtons, SIGNAL(timeout()), this, SLOT(updateMovedElements()));
	connect(this, SIGNAL(selectionChanged()), this, SLOT(deselectLabels()));
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
}

void EditorViewScene::setMVIface(EditorViewMViface *mvIface)
{
	mMVIface = mvIface;
	mClipboardHandler.setMVIface(mvIface);
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

	if (diagram.isNull()) {
		// Root diagram is not set, for example, current tab is disabled. No need
		// to do anything with mouse manager.
		return;
	}
	QList<qReal::Id> elements = mWindow->editorManager().elements(diagram);
	delete mMouseMovementManager;
	mMouseMovementManager = new gestures::MouseMovementManager(elements
			, &mWindow->editorManager(), mWindow->gesturesPainter());
	connect(mWindow, SIGNAL(currentIdealGestureChanged()), this, SLOT(drawIdealGesture()));
	connect(mWindow, SIGNAL(gesturesShowed()), this, SLOT(printElementsOfRootDiagram()));
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
	foreach (QGraphicsItem * const item, mSelectList) {
		item->setSelected(true);
	}
}

Element *EditorViewScene::getElem(qReal::Id const &id) const
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
		if (el) {
			if (canBeContainedBy(el->id(), id)) {
				node = el;
				break;
			}
		}
	}

	if (!node) {
		if (mHighlightNode) {
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
	if (mHighlightNode) {
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
	if (mHighlightNode) {
		mHighlightNode->erasePlaceholder(true);
		mHighlightNode = NULL;
	}
}

bool EditorViewScene::canBeContainedBy(qReal::Id const &container, qReal::Id const &candidate) const
{
	bool allowed = false;
	foreach (qReal::Id const &type, mWindow->editorManager().containedTypes(container.type())) {
		allowed = allowed || mWindow->editorManager().isParentOf(candidate, type);
	}

	return allowed;
}

int EditorViewScene::launchEdgeMenu(EdgeElement *edge, NodeElement *node
		, QPointF const &scenePos, bool canBeConnected, commands::CreateElementCommand **createCommand)
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
	QStringList const groups = mWindow->editorManager().paletteGroups(node->id(), node->id());

	foreach (PossibleEdge const &pEdge, edge->getPossibleEdges()) {
		// if pEdge.first.first is parent of node->id(), then add all children of pEdge.first.second to the list
		// and vice versa
		if (mWindow->editorManager().isParentOf(node->id(), pEdge.first.first)) {
			targets << mWindow->editorManager().allChildrenTypesOf(pEdge.first.second);
		}

		if (mWindow->editorManager().isParentOf(node->id(), pEdge.first.second)) {
			targets << mWindow->editorManager().allChildrenTypesOf(pEdge.first.first);
		}
	}

	QSet<QString> const targetsSet = targets.toSet();
	QMap<QString, QString> targetsInGroups;
	QStringList targetGroups;
	foreach (QString const &group, groups) {
		QStringList const groupsContents = mWindow->editorManager().paletteGroupList(
				node->id(), node->id(), group);
		foreach (QString const &elementInGroup, groupsContents) {
			if (targetsSet.contains(elementInGroup)) {
				targetsInGroups.insertMulti(group, elementInGroup);
				if (!targetGroups.contains(group)) {
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
			QString const friendlyName = mWindow->editorManager().friendlyName(id);
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

	if (canBeConnected) {
		edgeMenu->addSeparator();
		edgeMenu->addAction(tr("Connect with the current item"));
	}

	QAction *executed = edgeMenu->exec(QCursor::pos());

	int result = 0;
	if (executed) {
		if (executed == mWindow->actionDeleteFromDiagram()) {
			result = -1;
		} else if (!(executed->text() == tr("Discard"))
					&& !(executed->text() == tr("Connect with the current item"))) {
			result = 1;
			if (createCommand && mLastCreatedWithEdgeCommand) {
				*createCommand = mLastCreatedWithEdgeCommand;
			}
		}
	}

	foreach(QObject *object, toDelete) {
		delete object;
	}

	return result;
}

qReal::Id EditorViewScene::createElement(QString const &str)
{
	qReal::Id result = createElement(str, mCreatePoint, true, &mLastCreatedWithEdgeCommand);
	mLastCreatedWithEdge = getElem(result);
	mShouldReparentItems = false;
	return result;
}

qReal::Id EditorViewScene::createElement(QString const &str, QPointF const &scenePos, bool searchForParents
		, CreateElementCommand **createCommand, bool executeImmediately, QPointF const shiftToParent)
{
	Id typeId = Id::loadFromString(str);
	Id objectId(typeId.editor(),typeId.diagram(),typeId.element(),QUuid::createUuid().toString());

	QByteArray data;
	QMimeData *mimeData = new QMimeData();
	QDataStream stream(&data, QIODevice::WriteOnly);
	QString mimeType = QString("application/x-real-uml-data");
	QString uuid = objectId.toString();
	QString pathToItem = Id::rootId().toString();
	QString name = mWindow->editorManager().friendlyName(typeId);
	bool isFromLogicalModel = false;
	stream << uuid;
	stream << pathToItem;
	stream << name;
	stream << shiftToParent;
	stream << isFromLogicalModel;

	mimeData->setData(mimeType, data);
	createElement(mimeData, scenePos, searchForParents, createCommand, executeImmediately);
	delete mimeData;

	return objectId;
}

void EditorViewScene::createElement(QMimeData const *mimeData, QPointF const &scenePos , bool searchForParents
		, CreateElementCommand **createCommandPointer, bool executeImmediately)
{
	QByteArray itemData = mimeData->data("application/x-real-uml-data");
	QDataStream inStream(&itemData, QIODevice::ReadOnly);

	QString uuid = "";
	QString pathToItem = "";
	QString name = "";
	QPointF shiftToParent;
	QString explosionTargetUuid = "";
	bool isFromLogicalModel = false;
	inStream >> uuid;
	inStream >> pathToItem;
	inStream >> name;
	inStream >> shiftToParent;
	inStream >> isFromLogicalModel;
	inStream >> explosionTargetUuid;

	Id const id = Id::loadFromString(uuid);
	Id const explosionTarget = explosionTargetUuid.isEmpty()
			? Id()
			: Id::loadFromString(explosionTargetUuid);

	if (mMVIface->graphicalAssistApi()->editorManagerInterface().getPatternNames().contains(id.element())) {
		CreateGroupCommand *createGroupCommand = new CreateGroupCommand(
				*this, *mMVIface->logicalAssistApi(), *mMVIface->graphicalAssistApi()
				, mMVIface->rootId(), mMVIface->rootId(), id, isFromLogicalModel, scenePos);
		if (executeImmediately) {
			mController->execute(createGroupCommand);
		}
	} else {
		Element *newParent = NULL;

		ElementImpl const * const impl = mWindow->editorManager().elementImpl(id);
		bool const isNode = impl->isNode();
		delete impl;

		if (searchForParents) {
			// if element is node then we should look for parent for him
			if (isNode) {
				foreach (QGraphicsItem *item, items(scenePos - shiftToParent)) {
					NodeElement *el = dynamic_cast<NodeElement*>(item);
					if (el && canBeContainedBy(el->id(), id)) {
						newParent = el;
						break;
					}
				}
			}

			if (newParent && dynamic_cast<NodeElement*>(newParent)) {
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

		QPointF const position = !newParent
				? scenePos
				: newParent->mapToItem(newParent, newParent->mapFromScene(scenePos));

		Id const parentId = newParent ? newParent->id() : mMVIface->rootId();

		createSingleElement(id, name, isNode, position, parentId, isFromLogicalModel
				, explosionTarget, createCommandPointer, executeImmediately);

		NodeElement *parentNode = dynamic_cast<NodeElement*>(newParent);
		if (parentNode) {
			Element *nextNode = parentNode->getPlaceholderNextElement();
			if (nextNode) {
				mMVIface->graphicalAssistApi()->stackBefore(id, nextNode->id());
			}
		}
	}
}

void EditorViewScene::createSingleElement(Id const &id, QString const &name, bool isNode
		, QPointF const &position, Id const &parentId, bool isFromLogicalModel
		, Id const &explosionTarget, CreateElementCommand **createCommandPointer
		, bool executeImmediately)
{
	CreateElementCommand *createCommand = new CreateElementCommand(
				*mMVIface->logicalAssistApi()
				, *mMVIface->graphicalAssistApi()
				, mMVIface->rootId()
				, parentId
				, id
				, isFromLogicalModel
				, name
				, position);

	if (createCommandPointer) {
		(*createCommandPointer) = createCommand;
	}
	mExploser->handleCreationWithExplosion(createCommand, id, explosionTarget);
	if (executeImmediately) {
		if (isNode) {
			QSize const size = mMVIface->graphicalAssistApi()->editorManagerInterface().iconSize(id);
			commands::InsertIntoEdgeCommand *insertCommand = new commands::InsertIntoEdgeCommand(
					*this, *mMVIface->logicalAssistApi(), *mMVIface->graphicalAssistApi(), Id(), Id()
					, parentId, position, QPointF(size.width(), size.height()), isFromLogicalModel, createCommand);
			mController->execute(insertCommand);
		} else {
			mController->execute(createCommand);
		}
	}
}

EdgeElement * EditorViewScene::edgeForInsertion(QPointF const &scenePos)
{
	foreach (QGraphicsItem *item, items(scenePos)) {
		EdgeElement *edge = dynamic_cast<EdgeElement *>(item);
		if (edge && edge->isDividable()) {
			QSizeF portSize(kvadratik, kvadratik);
			QRectF startPort(edge->mapToScene(edge->line().first()) - QPointF(kvadratik / 2, kvadratik / 2), portSize);
			QRectF endPort(edge->mapToScene(edge->line().last()) - QPointF(kvadratik / 2, kvadratik / 2), portSize);
			if (!startPort.contains(scenePos) && !endPort.contains(scenePos)) {
				return edge;
			}
		}
	}
	return NULL;
}

void EditorViewScene::resolveOverlaps(NodeElement *node, QPointF const &scenePos
		, QPointF const &shift, QMap<qReal::Id, QPointF> &shifting) const
{
	QList<NodeElement*> closeNodes = getCloseNodes(node);
	foreach (NodeElement *closeNode, closeNodes) {
		if (shifting.contains(closeNode->id())) {
			continue;
		}

		QLineF offset(node->mapToScene(node->boundingRect().center())
				, closeNode->mapToScene(closeNode->boundingRect().center()));

		qreal coeff = (node->boundingRect().width() / 2) / qAbs(offset.length() * qCos(offset.angle()));
		if (qAbs(offset.y2() - offset.y1()) * coeff > node->boundingRect().height() / 2) {
			coeff = (node->boundingRect().height() / 2) / qAbs(offset.length() * qSin(offset.angle()));
		}
		QLineF nodeLine(offset);
		nodeLine.setP2(nodeLine.p1() + QPointF((nodeLine.p2().x() - nodeLine.p1().x()) * coeff
				, (nodeLine.p2().y() - nodeLine.p1().y()) * coeff));
		offset.setPoints(offset.p2(), offset.p1());

		coeff = (closeNode->boundingRect().width() / 2) / qAbs(offset.length() * qCos(offset.angle()));
		if (qAbs(offset.y2() - offset.y1()) * coeff > closeNode->boundingRect().height() / 2) {
			coeff = (closeNode->boundingRect().height() / 2) / qAbs(offset.length() * qSin(offset.angle()));
		}
		QLineF closeNodeLine(offset);
		closeNodeLine.setP2(closeNodeLine.p1() + QPointF((closeNodeLine.p2().x() - closeNodeLine.p1().x()) * coeff
				, (closeNodeLine.p2().y() - closeNodeLine.p1().y()) * coeff));

		QPointF offsetPoint(nodeLine.p2() - closeNodeLine.p2());
		closeNode->setPos(closeNode->pos() + offsetPoint);
		mMVIface->graphicalAssistApi()->setPosition(closeNode->id(), closeNode->pos());
		shifting.insert(closeNode->id(), offsetPoint);

		arrangeNodeLinks(closeNode);
		resolveOverlaps(closeNode, scenePos, shift, shifting);
	}
}

void EditorViewScene::returnElementsToOldPositions(const QMap<Id, QPointF> &shifting) const
{
	foreach (qReal::Id const &id, shifting.keys()) {
		NodeElement *node = getNodeById(id);
		node->setPos(node->pos() - shifting[id]);
		mMVIface->graphicalAssistApi()->setPosition(node->id(), node->pos());
	}
}

void EditorViewScene::reConnectLink(EdgeElement * edgeElem)
{
	edgeElem->connectToPort();
	QPolygonF line;
	line << edgeElem->line()[0] << edgeElem->line().last();
	edgeElem->setLine(line);
	edgeElem->layOut();
}

void EditorViewScene::arrangeNodeLinks(NodeElement* node) const
{
	node->arrangeLinks();
	foreach (EdgeElement* nodeEdge, node->edgeList()) {
		nodeEdge->adjustLink();
		nodeEdge->setGraphicApiPos();
		nodeEdge->saveConfiguration();
	}
	node->arrangeLinks();
	node->adjustLinks();
}

NodeElement* EditorViewScene::getNodeById(qReal::Id const &itemId) const
{
	foreach (QGraphicsItem *item, items()) {
		NodeElement *node = dynamic_cast<NodeElement*>(item);
		if (node && node->id() == itemId) {
			return node;
		}
	}
	return NULL;
}

EdgeElement* EditorViewScene::getEdgeById(qReal::Id const &itemId) const
{
	foreach (QGraphicsItem *item, items()) {
		EdgeElement *edge = dynamic_cast<EdgeElement*>(item);
		if (edge && edge->id() == itemId) {
			return edge;
		}
	}
	return NULL;
}

QList<NodeElement*> EditorViewScene::getCloseNodes(NodeElement *node) const
{
	QList<NodeElement *> list;

	if (node) {
		QPolygonF const bounds = node->mapToScene(node->boundingRect());
		QList<QGraphicsItem *> overlapping = items(bounds);
		foreach (QGraphicsItem * const item, overlapping) {
			NodeElement * const closeNode = dynamic_cast<NodeElement *>(item);
			if (closeNode && (closeNode != node) && !closeNode->isAncestorOf(node) && !node->isAncestorOf(closeNode)) {
				list.append(closeNode);
			}
		}
	}

	return list;
}

void EditorViewScene::copy()
{
	mClipboardHandler.copy();
}

void EditorViewScene::paste(bool isGraphicalCopy)
{
	mClipboardHandler.paste(isGraphicalCopy);
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

	if (!moveNodes()) {
		moveEdges();
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

bool EditorViewScene::moveNodes()
{
	bool movedNodesPresent = false;
	ResizeCommand *resizeCommand = NULL;

	foreach (QGraphicsItem * const item, selectedItems()) {
		NodeElement * const node = dynamic_cast<NodeElement *>(item);
		if (!node) {
			continue;
		}

		if (!resizeCommand) {
			resizeCommand = new ResizeCommand(this, node->id());
			resizeCommand->startTracking();
		}

		QPointF newPos = node->pos();
		newPos += mOffset;
		node->setPos(newPos);
		node->alignToGrid();
		node->adjustLinks();

		movedNodesPresent = true;
	}

	if (resizeCommand) {
		resizeCommand->stopTracking();
		mController->execute(resizeCommand);
	}

	return movedNodesPresent;
}

void EditorViewScene::moveEdges()
{
	foreach (QGraphicsItem * const item, selectedItems()) {
		EdgeElement * const edge = dynamic_cast<EdgeElement *>(item);
		if (edge) {
			ReshapeEdgeCommand * const edgeCommand = new ReshapeEdgeCommand(this, edge->id());
			edgeCommand->startTracking();

			QPointF newPos = edge->pos();
			newPos += mOffset;
			edge->setPos(newPos);
			if (edge && !(edge->src() && edge->dst()) && (edge->src() || edge->dst())
					&& (edge->src() ? !edge->src()->isSelected() : true)
					&& (edge->dst() ? !edge->dst()->isSelected() : true)) {
				edge->adjustLink();
			}

			edgeCommand->stopTracking();
			mController->execute(edgeCommand);
		}
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
	QGraphicsItem * item = itemAt(mCurrentMousePos, QTransform());
	if (event->modifiers() & Qt::ControlModifier) {
		if (item) {
			QGraphicsScene::mousePressEvent(event);
		}
	} else {
		QGraphicsScene::mousePressEvent(event);
	}

	if ((event->modifiers() & Qt::ControlModifier) && (event->buttons() & Qt::LeftButton)
			&& !(event->buttons() & Qt::RightButton))
	{
		mIsSelectEvent = true;
		mSelectList.append(selectedItems());
		foreach (QGraphicsItem * const item, items()) {
			item->setAcceptedMouseButtons(0);
		}

		foreach (QGraphicsItem * const item, mSelectList) {
			item->setSelected(true);
		}

		if (item) {
			item->setSelected(!mSelectList.contains(item));
			if (item->isSelected()) {
				mSelectList.append(item);
			} else {
				mSelectList.removeAll(item);
			}
		}
	} else if (event->button() == Qt::LeftButton) {
		mLeftButtonPressed = true;

		Label * const label = dynamic_cast<Label *>(item);
		if (label) {
			item = item->parentItem();
		}

		if (item) {
			item->setSelected(true);
			mSelectList.clear();
			mSelectList.append(item);
			event->accept();
		}
	} else if (event->button() == Qt::RightButton && !(event->buttons() & Qt::LeftButton)) {
		mTimer->stop();
		mMouseMovementManager->mousePress(event->scenePos());
		mRightButtonPressed = true;
	}

	redraw();

	mShouldReparentItems = (selectedItems().size() > 0);
}

void EditorViewScene::initContextMenu(Element *e, const QPointF &pos)
{
	if (mContextMenu.isVisible()) {
		mContextMenu.close();
	}

	disableActions(e);
	mContextMenu.clear();
	mContextMenu.addActions(mContextMenuActions);

	QSignalMapper *createChildMapper = NULL;
	if (e) {
		QList<ContextMenuAction*> elementActions = e->contextMenuActions(e->mapFromScene(pos));

		if (!elementActions.isEmpty()) {
			mContextMenu.addSeparator();
		}

		foreach (ContextMenuAction* action, elementActions) {
			action->setEventPos(e->mapFromScene(pos));
			mContextMenu.addAction(action);

			connect(action, SIGNAL(triggered()), mActionSignalMapper, SLOT(map()), Qt::UniqueConnection);
			mActionSignalMapper->setMapping(action, action->text() + "###" + e->id().toString());
		}

		if (e->createChildrenFromMenu() && !mWindow->editorManager().containedTypes(e->id().type()).empty()) {
			mCreatePoint = pos;
			QMenu *createChildMenu = mContextMenu.addMenu(tr("Add child"));
			createChildMapper = new QSignalMapper();
			foreach (Id const &type, mWindow->editorManager().containedTypes(e->id().type())) {
				QAction *createAction = createChildMenu->addAction(mWindow->editorManager().friendlyName(type));
				connect(createAction, SIGNAL(triggered()), createChildMapper, SLOT(map()), Qt::UniqueConnection);
				createChildMapper->setMapping(createAction, type.toString());
				connect(createChildMapper, SIGNAL(mapped(QString const &)), this, SLOT(createElement(QString const &)));
			}
		}

		mContextMenu.addSeparator();
		mExploser->createConnectionSubmenus(mContextMenu, e);
	}

	mContextMenu.exec(QCursor::pos());

	enableActions();
	delete createChildMapper;
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
				&& mWindow->editorManager().isParentOf(child.id().editor(), child.id().diagram()
						, possibleEdge.first.second.element(), child.id().diagram(), child.id().element())
				&& mWindow->editorManager().isParentOf(child.id().editor(), child.id().diagram()
						, possibleEdge.first.first.element(), child.id().diagram(), parent->id().element()))
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
		QAction *element = new QAction(mWindow->editorManager().friendlyName(Id::loadFromString(id)), edgeMenu);
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
	CreateElementCommand *createCommand;
	Id const id = createElement(idStr, start, true, &createCommand);
	Element *edgeElement = getElem(id);
	EdgeElement *edge = dynamic_cast <EdgeElement *>(edgeElement);
	edge->setSrc(NULL);
	edge->setDst(NULL);

	edge->placeEndTo(edge->mapFromScene(end));
	edge->connectToPort();
	if (edge->dst()) {
		edge->dst()->arrangeLinks();
		foreach (EdgeElement* nodeEdge, edge->dst()->edgeList()) {
			nodeEdge->adjustLink();
			nodeEdge->setGraphicApiPos();
			nodeEdge->saveConfiguration();
		}
		edge->dst()->arrangeLinks();
		edge->dst()->adjustLinks();
	}
	ReshapeEdgeCommand *reshapeEdgeCommand = new ReshapeEdgeCommand(this, id);
	reshapeEdgeCommand->startTracking();
	edge->layOut();
	reshapeEdgeCommand->stopTracking();
	reshapeEdgeCommand->setUndoEnabled(false);
	createCommand->addPostAction(reshapeEdgeCommand);
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
		foreach (QGraphicsItem* item, mSelectList) {
			item->setSelected(true);
		}
		mSelectList.clear();
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
		foreach(QGraphicsItem * const item, list) {
			NodeElement * const nodeItem = dynamic_cast<NodeElement *>(item);
			if (nodeItem) {
				Element * const e = dynamic_cast<Element *>(itemAt(event->scenePos(), QTransform()));
				if ((e && (nodeItem->id() != e->id())) || !e) {
					sendEvent(item, event);
				}
				if (list.size() > 1 && nodeItem) {
					nodeItem->setVisibleEmbeddedLinkers(false);
					nodeItem->setPortsVisible(QStringList());
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

QPointF EditorViewScene::getMousePos() const
{
	return mCurrentMousePos;
}

void EditorViewScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	Element *e = dynamic_cast<Element *>(itemAt(event->scenePos(), QTransform()));
	if (e && e->isSelected() && !event->modifiers()) {
		mainWindow()->graphicalModelExplorer()->setFocus();
	}
	QGraphicsScene::mouseDoubleClickEvent(event);

	if (event->button() == Qt::LeftButton && !event->modifiers()) {
		// Double click on a title activates it
		if (Label *title = dynamic_cast<Label*>(itemAt(event->scenePos(), QTransform()))) {
			if (!title->hasFocus()) {  // Do not activate already activated item
				event->accept();
				title->startTextInteraction();
				return;
			}
		} else if (NodeElement *element = dynamic_cast<NodeElement*>(itemAt(event->scenePos(), QTransform()))) {
			event->accept();
			mExploser->handleDoubleClick(element->logicalId());
		}
	}
}

Element *EditorViewScene::getElemAt(QPointF const &position) const
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
	mController = mWindow->controller();
	mClipboardHandler.setController(mController);
	mExploser = new view::details::ExploserView(mainWindow, &mainWindow->models()->logicalModelAssistApi()
			, &mainWindow->models()->graphicalModelAssistApi(), this);
	connect(mWindow, SIGNAL(rootDiagramChanged()), this, SLOT(initMouseMoveManager()));
	mContextMenuActions << mWindow->actionDeleteFromDiagram()
			<< mWindow->actionCopyElementsOnDiagram()
			<< mWindow->actionPasteOnDiagram() << mWindow->actionPasteCopyOfLogical();
}

qReal::MainWindow *EditorViewScene::mainWindow() const
{
	return mWindow;
}

void EditorViewScene::changePropertiesActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	PropertiesDialog * const propertiesDialog = new PropertiesDialog(*mWindow, mWindow->editorManager(), id);
	propertiesDialog->setModal(true);
	propertiesDialog->show();
}

void EditorViewScene::changeAppearanceActionTriggered()
{
	QAction const * const action = static_cast<QAction const *>(sender());
	Id const id = action->data().value<Id>();
	QString const propertyValue = mWindow->editorManager().shape(id);
	mWindow->openShapeEditor(id, propertyValue, &(mWindow->editorManager()));
}

void EditorViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (mNeedDrawGrid) {
		mWidthOfGrid = SettingsManager::value("GridWidth").toDouble() / 100;
		painter->setPen(QPen(Qt::black, mWidthOfGrid));

		int const indexGrid = SettingsManager::value("IndexGrid").toInt();
		mGridDrawer.drawGrid(painter, rect, indexGrid);
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
	mRightButtonPressed = false;
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
		if (mainWindow()->getCurrentTab()->scene()->items().contains(element))
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
		EdgeElement *const element = dynamic_cast<EdgeElement*>(item);
		if (element) {
			enums::linkShape::LinkShape const shape
					= static_cast<enums::linkShape::LinkShape>(SettingsManager::value("LineType"
							, enums::linkShape::unset).toInt());
			if (shape != enums::linkShape::unset) {
				element->changeShapeType(shape);
			}

			if (SettingsManager::value("ActivateGrid").toBool()) {
				element->alignToGrid();
			}
		}
	}
}

void EditorViewScene::initNodes()
{
	foreach (QGraphicsItem *item, items()) {
		NodeElement* node = dynamic_cast<NodeElement*>(item);
		if (node) {
			node->adjustLinks();
			if (mMVIface->graphicalAssistApi()->properties(node->id()).contains("expanded")
					&& mMVIface->graphicalAssistApi()->graphicalRepoApi().property(
							node->id(), "expanded").toString() == "true") {
				node->changeExpanded();
			}
			if (mMVIface->graphicalAssistApi()->properties(node->id()).contains("folded")
					&& mMVIface->graphicalAssistApi()->graphicalRepoApi().property(
							node->id(), "folded").toString() == "true") {
				node->changeFoldState();
			}

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

void EditorViewScene::deselectLabels()
{
	foreach (QGraphicsItem *item, items()) {
		Label *label = dynamic_cast<Label *>(item);
		if (label && !label->isSelected()) {
			label->clearMoveFlag();
		}
	}
}
