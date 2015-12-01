/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "editorViewScene.h"

#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsTextItem>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <math.h>
#include <qmath.h>

#include <qrkernel/logging.h>
#include <qrgui/models/models.h>
#include <qrgui/mouseGestures/mouseMovementManager.h>
#include <qrgui/mouseGestures/dummyMouseMovementManager.h>

#include "editor/sceneCustomizer.h"
#include "editor/labels/label.h"
#include "editor/commands/multipleRemoveAndUpdateCommand.h"
#include "editor/commands/createAndUpdateGroupCommand.h"
#include "editor/commands/insertIntoEdgeCommand.h"
#include "editor/commands/reshapeEdgeCommand.h"
#include "editor/commands/resizeCommand.h"
#include "editor/commands/expandCommand.h"

using namespace qReal;
using namespace qReal::commands;
using namespace qReal::gui::editor;
using namespace qReal::gui::editor::commands;

EditorViewScene::EditorViewScene(const models::Models &models
		, Controller &controller
		, const SceneCustomizer &sceneCustomizer
		, const Customizer &customizer
		, const Id &rootId
		, QObject *parent)
	: QGraphicsScene(parent)
	, mModels(models)
	, mEditorManager(models.logicalModelAssistApi().editorManagerInterface())
	, mController(controller)
	, mSceneCustomizer(sceneCustomizer)
	, mCustomizer(customizer)
	, mRootId(rootId)
	, mLastCreatedFromLinker(nullptr)
	, mClipboardHandler(*this, controller)
	, mRightButtonPressed(false)
	, mLeftButtonPressed(false)
	, mHighlightNode(nullptr)
	, mMouseMovementManager(new gestures::DummyMouseMovementManager(mRootId, mEditorManager))
	, mActionSignalMapper(new QSignalMapper(this))
	, mTimer(new QTimer(this))
	, mTimerForArrowButtons(new QTimer(this))
	, mOffset(QPointF(0, 0))
	, mShouldReparentItems(false)
	, mTopLeftCorner(new QGraphicsRectItem(0, 0, 1, 1))
	, mBottomRightCorner(new QGraphicsRectItem(0, 0, 1, 1))
	, mMouseGesturesEnabled(false)
	, mExploser(models, controller, sceneCustomizer, customizer, this)
	, mActionDeleteFromDiagram(nullptr)
	, mActionCutOnDiagram(nullptr)
	, mActionCopyOnDiagram(nullptr)
	, mActionPasteOnDiagram(nullptr)
	, mActionPasteReference(nullptr)

{
	mNeedDrawGrid = SettingsManager::value("ShowGrid").toBool();
	mWidthOfGrid = static_cast<qreal>(SettingsManager::value("GridWidth").toInt()) / 100;
	mRealIndexGrid = SettingsManager::value("IndexGrid").toInt();

	setItemIndexMethod(NoIndex);
	setEnabled(false);

	initCorners();
	initializeActions();

	connect(mTimer, SIGNAL(timeout()), this, SLOT(getObjectByGesture()));
	connect(mTimerForArrowButtons, SIGNAL(timeout()), this, SLOT(updateMovedElements()));
	connect(this, &QGraphicsScene::selectionChanged, this, &EditorViewScene::deselectLabels);
	connect(&mExploser, &view::details::ExploserView::goTo, this, &EditorViewScene::goTo);
	connect(&mExploser, &view::details::ExploserView::refreshPalette, this, &EditorViewScene::refreshPalette);
	connect(&mExploser, &view::details::ExploserView::openShapeEditor, this, &EditorViewScene::openShapeEditor);
	connect(&mExploser, &view::details::ExploserView::expandElement, [=](const Id &element) {
		if (NodeElement * const node = getNodeById(element)) {
			mController.execute(new ExpandCommand(node));
		}
	});
}

qreal EditorViewScene::realIndexGrid()
{
	return mRealIndexGrid;
}

void EditorViewScene::setRealIndexGrid(qreal newIndexGrid)
{
	mRealIndexGrid = newIndexGrid;
}

void EditorViewScene::setEnabled(bool enabled)
{
	for (QGraphicsView *view : views()) {
		view->setEnabled(enabled);
	}
}

void EditorViewScene::clearScene()
{
	QList<QGraphicsItem*> itemsForRemoving;
	for (QGraphicsItem *item : items()) {
		// looks really insane, but some elements were alreadt deleted together with their parent
		if (items().contains(item) && !(item == mTopLeftCorner || item == mBottomRightCorner)) {
			itemsForRemoving.append(item);
		}
	}

	for (QGraphicsItem *item : itemsForRemoving) {
		removeItem(item);
	}
}

Element *EditorViewScene::getElem(const Id &id) const
{
	if (id == Id::rootId()) {
		return nullptr;
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
	return nullptr;
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

	NodeElement *node = nullptr;
	for (QGraphicsItem *item : elements) {
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
	if (prevHighlighted != mHighlightNode && prevHighlighted != nullptr) {
		prevHighlighted->erasePlaceholder(true);
	}
}

NodeElement *EditorViewScene::findNewParent(QPointF newParentInnerPoint, NodeElement *node)
{
	QList<QGraphicsItem *> selected = selectedItems();
	const Id &id = node->id();

	// when we select multiple elements and move them, position of mouse release event could be
	// exactly over one of them. so to prevent handling this situation as putting all others in
	// container, we check if new parent is selected right now.
	if (nullptr == node->parentItem() || !selected.contains(node->parentItem())) {

		// if we want to put multiple elements in a container, we should take scene()->items()
		// and remove elements that are currently selected from it.

		// but there could be a situation that we're trying to move element and it's container
		// together. it that case we should not change parent of this element

		// delete from parents list ones that are selected right now
		// we get the first valid NodeElement
		for (QGraphicsItem *item : items(newParentInnerPoint)) {
			NodeElement *e = dynamic_cast<NodeElement *>(item);
			if (e && e != node && !selected.contains(item)) {
				// check if we can add element into found parent
				if (canBeContainedBy(e->id(), id)) {
					return e;
				}
			}

		}
	}

	return nullptr;
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
		mHighlightNode = nullptr;
	}
}

void EditorViewScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	// if there's no diagrams. create nothing
	if (!mModels.graphicalModelAssistApi().hasRootDiagrams()) {
		return;
	}

	createElement(event->mimeData(), event->scenePos());
	if (mHighlightNode) {
		mHighlightNode->erasePlaceholder(true);
		mHighlightNode = nullptr;
	}
}

bool EditorViewScene::canBeContainedBy(const Id &container, const Id &candidate) const
{
	bool allowed = false;
	for (const Id &type : mEditorManager.containedTypes(container.type())) {
		allowed = allowed || mEditorManager.isParentOf(candidate, type);
	}

	return allowed;
}

int EditorViewScene::launchEdgeMenu(EdgeElement *edge, NodeElement *node
		, const QPointF &scenePos, bool canBeConnected, CreateElementCommand **createCommand)
{
	edge->setSelected(true);

	QList<QObject*> toDelete;

	QMenu *edgeMenu = new QMenu();
	toDelete.append(edgeMenu);
	edgeMenu->addAction(&mActionDeleteFromDiagram);
	edgeMenu->addSeparator();

	QMenu *createElemMenu = new QMenu(tr("Create new element"), edgeMenu); // deleted as child of edgeMenu
	edgeMenu->addMenu(createElemMenu);

	QSignalMapper *menuSignalMapper = new QSignalMapper();
	toDelete.append(menuSignalMapper);

	QStringList targets;
	const QStringList groups = mEditorManager.paletteGroups(node->id(), node->id());

	for (const PossibleEdge &pEdge : edge->getPossibleEdges()) {
		// if pEdge.first.first is parent of node->id(), then add all children of pEdge.first.second to the list
		// and vice versa
		if (mEditorManager.isParentOf(node->id(), pEdge.first.first)) {
			targets << mEditorManager.allChildrenTypesOf(pEdge.first.second);
		}

		if (mEditorManager.isParentOf(node->id(), pEdge.first.second)) {
			targets << mEditorManager.allChildrenTypesOf(pEdge.first.first);
		}
	}

	QSet<QString> const targetsSet = targets.toSet();
	QMap<QString, QString> targetsInGroups;
	QStringList targetGroups;
	for (const QString &group : groups) {
		const QStringList groupsContents = mEditorManager.paletteGroupList(
				node->id(), node->id(), group);
		for (const QString &elementInGroup : groupsContents) {
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
		const QStringList targetsInGroup = targetsInGroups.values(targetGroups[i]);
		for (const QString &target : targetsInGroup) {
			const Id id = Id::loadFromString("qrm:/" + node->id().editor() + "/" + node->id().diagram() + "/" + target);
			const QString friendlyName = mEditorManager.friendlyName(id);
			QAction *element = new QAction(friendlyName, createElemMenu);
			// deleted as child of createElemMenu
			createElemMenu->addAction(element);
			QObject::connect(element,SIGNAL(triggered()), menuSignalMapper, SLOT(map()));
			menuSignalMapper->setMapping(element, id.toString());
		}
	}

	mCreatePoint = scenePos;
	mLastCreatedFromLinker = Id();
	QObject::connect(menuSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(createElement(const QString &)));

	if (canBeConnected) {
		edgeMenu->addSeparator();
		edgeMenu->addAction(tr("Connect with the current item"));
	}

	QAction *executed = edgeMenu->exec(QCursor::pos());

	int result = 0;
	if (executed) {
		if (executed == &mActionDeleteFromDiagram) {
			result = -1;
		} else if (!(executed->text() == tr("Connect with the current item"))) {
			result = 1;
			if (createCommand && mLastCreatedFromLinkerCommand) {
				*createCommand = mLastCreatedFromLinkerCommand;
			}
		}
	}

	qDeleteAll(toDelete);

	return result;
}

Id EditorViewScene::createElement(const QString &str)
{
	mLastCreatedFromLinker = createElement(str, mCreatePoint, true, &mLastCreatedFromLinkerCommand);
	mShouldReparentItems = false;
	return mLastCreatedFromLinker;
}

Id EditorViewScene::createElement(const QString &str
		, const QPointF &scenePos
		, bool searchForParents
		, CreateElementCommand **createCommand
		, bool executeImmediately
		, const QPointF &shiftToParent
		, const QString &explosionTargetUuid)
{
	Id typeId = Id::loadFromString(str);
	Id objectId(typeId.editor(), typeId.diagram(), typeId.element(), QUuid::createUuid().toString());

	QByteArray data;
	QMimeData *mimeData = new QMimeData();
	QDataStream stream(&data, QIODevice::WriteOnly);
	QString mimeType = QString("application/x-real-uml-data");
	QString uuid = objectId.toString();
	QString pathToItem = Id::rootId().toString();
	QString name = mEditorManager.friendlyName(typeId);
	bool isFromLogicalModel = false;
	stream << uuid;
	stream << pathToItem;
	stream << name;
	stream << shiftToParent;
	stream << isFromLogicalModel;
	stream << explosionTargetUuid;

	mimeData->setData(mimeType, data);
	createElement(mimeData, scenePos, searchForParents, createCommand, executeImmediately);
	delete mimeData;

	return objectId;
}

void EditorViewScene::createElement(const QMimeData *mimeData, const QPointF &scenePos , bool searchForParents
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

	const Id id = Id::loadFromString(uuid);

	if (!mEditorManager.hasElement(id.type())) {
		return;
	}

	QLOG_TRACE() << "Created element, id = " << id << ", position = " << scenePos;

	const Id explosionTarget = explosionTargetUuid.isEmpty()
			? Id()
			: Id::loadFromString(explosionTargetUuid);

	if (mEditorManager.getPatternNames().contains(id.element())) {
		CreateAndUpdateGroupCommand *createGroupCommand = new CreateAndUpdateGroupCommand(
				*this, mModels.logicalModelAssistApi(), mModels.graphicalModelAssistApi(), mModels.exploser()
				, mRootId, mRootId, id, isFromLogicalModel, scenePos);
		if (executeImmediately) {
			mController.execute(createGroupCommand);
		}
	} else {
		Element *newParent = nullptr;

		const ElementImpl * const impl = mEditorManager.elementImpl(id);
		const bool isNode = impl->isNode();
		delete impl;

		if (searchForParents) {
			// if element is node then we should look for parent for him
			if (isNode) {
				for (QGraphicsItem *item : items(scenePos - shiftToParent)) {
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
					text += "Element of type \"" + id.element() + "\" can not be a child of \""
							+ newParent->id().element() + "\"";

					QMessageBox::critical(0, "Error!", text);
					return;
				}

				//temporary solution for chaotic changes of coordinates of created elements with edge menu
				if (dynamic_cast<EdgeElement*>(newParent)) {
					newParent = nullptr;
				}
			}
		}

		const QPointF position = !newParent
				? scenePos
				: newParent->mapToItem(newParent, newParent->mapFromScene(scenePos));

		const Id parentId = newParent ? newParent->id() : mRootId;

		createSingleElement(id, name, isNode, position, parentId, isFromLogicalModel
				, explosionTarget, createCommandPointer, executeImmediately);

		NodeElement *parentNode = dynamic_cast<NodeElement*>(newParent);
		if (parentNode) {
			Element *nextNode = parentNode->getPlaceholderNextElement();
			if (nextNode) {
				mModels.graphicalModelAssistApi().stackBefore(id, nextNode->id());
			}
		}
	}
}

void EditorViewScene::createSingleElement(const Id &id, const QString &name, bool isNode
		, const QPointF &position, const Id &parentId, bool isFromLogicalModel
		, const Id &explosionTarget, CreateElementCommand **createCommandPointer
		, bool executeImmediately)
{
	CreateElementCommand *createCommand = new CreateElementCommand(
				mModels.logicalModelAssistApi()
				, mModels.graphicalModelAssistApi()
				, mModels.exploser()
				, mRootId
				, parentId
				, id
				, isFromLogicalModel
				, name
				, position);

	if (createCommandPointer) {
		(*createCommandPointer) = createCommand;
	}

	mExploser.handleCreationWithExplosion(createCommand, id, explosionTarget);
	if (executeImmediately) {
		if (isNode) {
			const QSize size = mEditorManager.iconSize(id);
			commands::InsertIntoEdgeCommand *insertCommand = new commands::InsertIntoEdgeCommand(
					*this, mModels.logicalModelAssistApi(), mModels.graphicalModelAssistApi(), mModels.exploser()
					, Id(), Id(), parentId, position, QPointF(size.width(), size.height())
					, isFromLogicalModel, createCommand);
			mController.execute(insertCommand);
			if (explosionTarget != Id()) {
				getNodeById(id)->updateShape();
			}
		} else {
			mController.execute(createCommand);
		}
	}
}

EdgeElement * EditorViewScene::edgeForInsertion(const QPointF &scenePos)
{
	for (QGraphicsItem *item : items(scenePos)) {
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

	return nullptr;
}

void EditorViewScene::resolveOverlaps(NodeElement *node, const QPointF &scenePos
		, const QPointF &shift, QMap<Id, QPointF> &shifting) const
{
	QList<NodeElement*> closeNodes = getCloseNodes(node);
	for (NodeElement *closeNode : closeNodes) {
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
		mModels.graphicalModelAssistApi().setPosition(closeNode->id(), closeNode->pos());
		shifting.insert(closeNode->id(), offsetPoint);

		arrangeNodeLinks(closeNode);
		resolveOverlaps(closeNode, scenePos, shift, shifting);
	}
}

void EditorViewScene::returnElementsToOldPositions(const QMap<Id, QPointF> &shifting) const
{
	for (const Id &id : shifting.keys()) {
		NodeElement *node = getNodeById(id);
		node->setPos(node->pos() - shifting[id]);
		mModels.graphicalModelAssistApi().setPosition(node->id(), node->pos());
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
	for (EdgeElement* nodeEdge : node->edgeList()) {
		nodeEdge->adjustLink();
		nodeEdge->setGraphicApiPos();
		nodeEdge->saveConfiguration();
	}

	node->arrangeLinks();
	node->adjustLinks();
}

NodeElement* EditorViewScene::getNodeById(const Id &itemId) const
{
	for (QGraphicsItem *item : items()) {
		NodeElement *node = dynamic_cast<NodeElement*>(item);
		if (node && node->id() == itemId) {
			return node;
		}
	}

	return nullptr;
}

EdgeElement* EditorViewScene::getEdgeById(const Id &itemId) const
{
	for (QGraphicsItem *item : items()) {
		EdgeElement *edge = dynamic_cast<EdgeElement*>(item);
		if (edge && edge->id() == itemId) {
			return edge;
		}
	}

	return nullptr;
}

QList<NodeElement*> EditorViewScene::getCloseNodes(NodeElement *node) const
{
	QList<NodeElement *> list;

	if (node) {
		const QPolygonF bounds = node->mapToScene(node->boundingRect());
		QList<QGraphicsItem *> overlapping = items(bounds);
		for (QGraphicsItem * const item : overlapping) {
			NodeElement * const closeNode = dynamic_cast<NodeElement *>(item);
			if (closeNode && (closeNode != node) && !closeNode->isAncestorOf(node) && !node->isAncestorOf(closeNode)) {
				list.append(closeNode);
			}
		}
	}

	return list;
}

void EditorViewScene::cut()
{
	mClipboardHandler.cut();
}

void EditorViewScene::copy()
{
	mClipboardHandler.copy();
}

void EditorViewScene::paste(bool isGraphicalCopy)
{
	mClipboardHandler.paste(isGraphicalCopy);
}

Element *EditorViewScene::lastCreatedFromLinker() const
{
	return getElem(mLastCreatedFromLinker);
}

void EditorViewScene::deleteSelectedItems()
{
	QList<QGraphicsItem *> itemsToDelete = selectedItems();
	IdList idsToDelete;
	for (const QGraphicsItem *item : itemsToDelete) {
		const Element *element = dynamic_cast<const Element *>(item);
		if (element) {
			idsToDelete << element->id();
		}
	}

	if (!idsToDelete.isEmpty()) {
		deleteElements(idsToDelete);
	}
}

void EditorViewScene::deleteElements(IdList &idsToDelete)
{
	MultipleRemoveAndUpdateCommand * const command = new MultipleRemoveAndUpdateCommand(*this, mModels);
	command->setItemsToDelete(idsToDelete);
	mController.execute(command);
}

void EditorViewScene::keyPressEvent(QKeyEvent *event)
{
	if (dynamic_cast<QGraphicsTextItem *>(focusItem())) {
		// Forward event to text editor
		QGraphicsScene::keyPressEvent(event);
	} else if (isArrow(event->key())) {
		moveSelectedItems(event->key());
	} else if (event->key() == Qt::Key_Menu) {
		initContextMenu(nullptr, QPointF()); // see #593
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

void EditorViewScene::keyReleaseEvent(QKeyEvent *event)
{
	if (isArrow(event->key()) && !selectedItems().isEmpty()) {
		event->accept();
		return;
	}

	QGraphicsScene::keyReleaseEvent(event);
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
	ResizeCommand *resizeCommand = nullptr;

	for (QGraphicsItem * const item : selectedItems()) {
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
		mController.execute(resizeCommand);
	}

	return movedNodesPresent;
}

void EditorViewScene::moveEdges()
{
	for (QGraphicsItem * const item : selectedItems()) {
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
			mController.execute(edgeCommand);
		}
	}
}

void EditorViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);

	mCurrentMousePos = event->scenePos();
	QGraphicsItem * item = itemAt(mCurrentMousePos, QTransform());

	if (event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
		mLeftButtonPressed = true;

		Label * const label = dynamic_cast<Label *>(item);
		if (label) {
			item = item->parentItem();
		}

		if (item) {
			event->accept();
		}
	} else if (event->button() == Qt::RightButton && !(event->buttons() & Qt::LeftButton) && mMouseGesturesEnabled) {
		mTimer->stop();

		const QPoint pos = views()[0]->window()->mapFromGlobal(event->screenPos());
		QLOG_TRACE() << "Started mouse gesture at " << pos;

		mMouseMovementManager->mousePress(event->scenePos());
		mRightButtonPressed = true;
	}

	invalidate();

	mShouldReparentItems = (selectedItems().size() > 0);
}

void EditorViewScene::initContextMenu(Element *e, const QPointF &pos)
{
	if (mContextMenu.isVisible()) {
		mContextMenu.close();
	}

	disableActions(e);
	mContextMenu.clear();
	mContextMenu.addAction(&mActionDeleteFromDiagram);
	mContextMenu.addSeparator();
	mContextMenu.addActions(mEditorActions);

	QSignalMapper *createChildMapper = nullptr;
	if (e) {
		if (e->createChildrenFromMenu() && !mEditorManager.containedTypes(e->id().type()).empty()) {
			mCreatePoint = pos;
			QMenu *createChildMenu = mContextMenu.addMenu(tr("Add child"));
			createChildMapper = new QSignalMapper();
			for (const Id &type : mEditorManager.containedTypes(e->id().type())) {
				QAction *createAction = createChildMenu->addAction(mEditorManager.friendlyName(type));
				connect(createAction, SIGNAL(triggered()), createChildMapper, SLOT(map()), Qt::UniqueConnection);
				createChildMapper->setMapping(createAction, type.toString());
			}

			connect(createChildMapper, SIGNAL(mapped(const QString &)), this, SLOT(createElement(const QString &)));
		}

		mContextMenu.addSeparator();
		mExploser.createConnectionSubmenus(mContextMenu, e);
	}

	mContextMenu.exec(QCursor::pos());

	setActionsEnabled(true);
	delete createChildMapper;
}

void EditorViewScene::disableActions(Element *focusElement)
{
	if (!focusElement) {
		mActionDeleteFromDiagram.setEnabled(false);
		mActionCopyOnDiagram.setEnabled(false);
		mActionCutOnDiagram.setEnabled(false);
	}
	if (isEmptyClipboard()) {
		mActionPasteOnDiagram.setEnabled(false);
		mActionPasteReference.setEnabled(false);
	}
}

bool EditorViewScene::isEmptyClipboard()
{
	const QMimeData* mimeData = QApplication::clipboard()->mimeData();
	return mimeData->data("application/x-real-uml-model-data").isEmpty();
}

void EditorViewScene::getObjectByGesture()
{
	mTimer->stop();
	const gestures::MouseMovementManager::GestureResult result = mMouseMovementManager->result();
	switch (result.type()) {
	case gestures::MouseMovementManager::invalidGesture:
		break;
	case gestures::MouseMovementManager::createElementGesture: {
		// Creating element with its center in the center of gesture (see #1086)
		const Id id = result.elementType();
		const QSize elementSize = mEditorManager.iconSize(id);
		const QPointF gestureCenter = mMouseMovementManager->pos();
		const QPointF elementCenter(elementSize.width() / 2.0, elementSize.height() / 2.0);
		createElement(id.toString(), gestureCenter - elementCenter);
		break;
	}
	case gestures::MouseMovementManager::deleteGesture:
		// Deleting element under the gesture center
		const QPointF gestureCenter = mMouseMovementManager->pos();
		for (QGraphicsItem * const item : items(gestureCenter)) {
			if (NodeElement * const node = dynamic_cast<NodeElement *>(item)) {
				deleteElements(IdList() << node->id());
				break;
			}
		}

		break;
	}

	deleteGesture();
}

void EditorViewScene::updateMovedElements()
{
	mTimerForArrowButtons->stop();

	if (mOffset == QPointF(0, 0)) {
		return;
	}

	for (QGraphicsItem* item : selectedItems()) {
		QPointF newPos = item->pos();

		Element* element = dynamic_cast<Element*>(item);
		if (element) {
			mModels.graphicalModelAssistApi().setPosition(element->id(), newPos);
		}
	}
}

void EditorViewScene::getLinkByGesture(NodeElement *parent, const NodeElement &child)
{
	QList<PossibleEdge> edges = parent->getPossibleEdges();
	QList<QString> allLinks;
	for (const PossibleEdge &possibleEdge : edges) {
		if (possibleEdge.first.second.editor() == child.id().editor()
				&& possibleEdge.first.second.diagram() == child.id().diagram()
				&& mEditorManager.isParentOf(child.id().editor(), child.id().diagram()
						, possibleEdge.first.second.element(), child.id().diagram(), child.id().element())
				&& mEditorManager.isParentOf(child.id().editor(), child.id().diagram()
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
	for (QString id : ids) {
		QAction *element = new QAction(mEditorManager.friendlyName(Id::loadFromString(id)), edgeMenu);
		edgeMenu->addAction(element);
		QObject::connect(element, SIGNAL(triggered()), menuSignalMapper, SLOT(map()));
		menuSignalMapper->setMapping(element, id);
	}

	QObject::connect(menuSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(createEdge(QString)));
	edgeMenu->exec(QCursor::pos());
}

void EditorViewScene::createEdge(const QString &idStr)
{
	const QPointF start = mMouseMovementManager->firstPoint();
	const QPointF end = mMouseMovementManager->lastPoint();
	CreateElementCommand *createCommand;
	const Id id = createElement(idStr, start, true, &createCommand);
	Element *edgeElement = getElem(id);
	EdgeElement *edge = dynamic_cast <EdgeElement *>(edgeElement);
	edge->setSrc(nullptr);
	edge->setDst(nullptr);

	edge->setPos(start);
	edge->placeStartTo(QPointF());
	edge->placeEndTo(edge->mapFromScene(end));
	edge->connectToPort();
	if (edge->dst()) {
		edge->dst()->arrangeLinks();
		for (EdgeElement* nodeEdge : edge->dst()->edgeList()) {
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

	QGraphicsScene::mouseReleaseEvent(event);
	Element *element = findElemAt(event->scenePos());

	if (event->button() == Qt::RightButton && !(mMouseMovementManager->pathIsEmpty())) {
		const QPoint pos = views()[0]->window()->mapFromGlobal(event->screenPos());
		QLOG_TRACE() << "Mouse gesture movement to " << pos;
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
				clearSelection();
				element->setSelected(true);
			}

			initContextMenu(element, event->scenePos());
			clearSelection();
			return;
		}

		QLOG_TRACE() << "Mouse gesture release at " << pos;

		const QPointF start = mMouseMovementManager->firstPoint();
		const QPointF end = mMouseMovementManager->lastPoint();
		NodeElement * const startNode = findNodeAt(start);
		NodeElement * const endNode = findNodeAt(end);
		if (startNode && endNode && mMouseMovementManager->isEdgeCandidate()
				&& startNode->id() != endNode->id()) {
			getLinkByGesture(startNode, *endNode);
			deleteGesture();
		} else {
			mTimer->start(SettingsManager::value("gestureDelay").toInt());
		}

		return;
	}

	if (element) {
		// To remove guides from scene
		invalidate();
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
			const QPoint pos = views()[0]->window()->mapFromGlobal(event->screenPos());
			QLOG_TRACE() << "Mouse gesture movement to " << pos;

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
	QGraphicsScene::mouseDoubleClickEvent(event);

	if (event->button() == Qt::LeftButton && !event->modifiers()) {
		// Double click on a title activates it
		for (QGraphicsItem * const item : items(event->scenePos())) {
			if (Label * const label = dynamic_cast<Label*>(item)) {
				if (!label->hasFocus() && !label->isReadOnly()) {  // Do not activate already activated or readonly item
					event->accept();
					label->startTextInteraction();
					return;
				}
			} else if (NodeElement *element = dynamic_cast<NodeElement*>(itemAt(event->scenePos(), QTransform()))) {
				event->accept();
				mExploser.handleDoubleClick(element->logicalId());
			}
		}
	}
}

Element *EditorViewScene::findElemAt(const QPointF &position) const
{
	for (QGraphicsItem * const item : items(position)) {
		if (Element * const element = dynamic_cast<Element *>(item)) {
			return element;
		}
	}

	return nullptr;
}

NodeElement *EditorViewScene::findNodeAt(const QPointF &position) const
{
	for (QGraphicsItem * const item : items(position)) {
		if (NodeElement * const node = dynamic_cast<NodeElement *>(item)) {
			return node;
		}
	}

	return nullptr;
}

Id EditorViewScene::rootItemId() const
{
	return mRootId;
}

const models::Models &EditorViewScene::models() const
{
	return mModels;
}

Controller &EditorViewScene::controller() const
{
	return mController;
}

const EditorManagerInterface &EditorViewScene::editorManager() const
{
	return mEditorManager;
}

const SceneCustomizer &EditorViewScene::sceneCustomizer() const
{
	return mSceneCustomizer;
}

const Customizer &EditorViewScene::customizer() const
{
	return mCustomizer;
}

QWidget *EditorViewScene::gesturesPainterWidget() const
{
	return mMouseMovementManager->producePainter();
}

void EditorViewScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (mNeedDrawGrid) {
		mWidthOfGrid = SettingsManager::value("GridWidth").toDouble() / 100;
		painter->setPen(QPen(Qt::black, mWidthOfGrid));

		const int indexGrid = SettingsManager::value("IndexGrid").toInt();
		mGridDrawer.drawGrid(painter, rect, indexGrid);
	}
}

void EditorViewScene::setNeedDrawGrid(bool show)
{
	mNeedDrawGrid = show;
}

void EditorViewScene::drawGesture()
{
	QLineF line = mMouseMovementManager->newLine();
	QGraphicsLineItem *item = new QGraphicsLineItem(line);
	qreal size = mGesture.size() * 0.1;
	qreal color_ratio = pow(fabs(sin(size)), 1.5);
	QColor penColor(255 * color_ratio, 255 * (1 - color_ratio), 255);
	item->setPen(penColor);
	addItem(item);
	mGesture.push_back(item);
}

void EditorViewScene::deleteGesture()
{
	mRightButtonPressed = false;
	QList<QGraphicsItem*> itemsForRemoving;
	for (QGraphicsItem *item : mGesture) {
		itemsForRemoving.append(item);
	}

	for (QGraphicsItem *item : itemsForRemoving) {
		removeItem(item);
	}

	mGesture.clear();
	mMouseMovementManager->clear();
}

void EditorViewScene::redraw()
{
	update();
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
}

void EditorViewScene::highlight(const Id &graphicalId, bool exclusive, const QColor &color)
{
	if (exclusive) {
		for (Element *element : mHighlightedElements) {
			element->setGraphicsEffect(nullptr);
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

void EditorViewScene::dehighlight(const Id &graphicalId)
{
	Element *elem = getElem(graphicalId);
	if (!elem) {
		return;
	}

	elem->setGraphicsEffect(nullptr);
	mHighlightedElements.remove(elem);
	elem->updateEnabledState();
}

void EditorViewScene::dehighlight()
{
	for (Element *element : mHighlightedElements) {
		if (items().contains(element))
			element->setGraphicsEffect(nullptr);
	}

	mHighlightedElements.clear();
}

void EditorViewScene::selectAll()
{
	if (!mLeftButtonPressed) {
		for (QGraphicsItem *element : items()) {
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

void EditorViewScene::setCorners(const QPointF &topLeft, const QPointF &bottomRight)
{
	mTopLeftCorner->setPos(topLeft);
	mBottomRightCorner->setPos(bottomRight);

	addItem(mTopLeftCorner);
	addItem(mBottomRightCorner);
}

void EditorViewScene::initializeActions()
{
	QAction * const separator = new QAction(this);
	separator->setSeparator(true);

	mActionDeleteFromDiagram.setShortcut(QKeySequence(Qt::Key_Delete));
	mActionDeleteFromDiagram.setText(tr("Delete"));
	connect(&mActionDeleteFromDiagram, &QAction::triggered, this, &EditorViewScene::deleteSelectedItems);

	mActionCopyOnDiagram.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	mActionCopyOnDiagram.setText(tr("Copy"));
	connect(&mActionCopyOnDiagram, &QAction::triggered, this, &EditorViewScene::copy);

	mActionPasteOnDiagram.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
	mActionPasteOnDiagram.setText(tr("Paste"));
	connect(&mActionPasteOnDiagram, &QAction::triggered, [=]() { paste(false); });

	mActionPasteReference.setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V));
	mActionPasteReference.setText(tr("Paste only graphical copy"));
	connect(&mActionPasteReference, &QAction::triggered, [=]() { paste(true); });

	mActionCutOnDiagram.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	mActionCutOnDiagram.setText(tr("Cut"));
	connect(&mActionCutOnDiagram, &QAction::triggered, this, &EditorViewScene::cut);

	mEditorActions << &mActionCutOnDiagram
			<< &mActionCopyOnDiagram
			<< &mActionPasteOnDiagram
			<< &mActionPasteReference;
}

void EditorViewScene::updateEdgeElements()
{
	for (QGraphicsItem *item : items()) {
		EdgeElement *const element = dynamic_cast<EdgeElement*>(item);
		if (element) {
			const enums::linkShape::LinkShape shape
					= static_cast<enums::linkShape::LinkShape>(SettingsManager::value("LineType").toInt());

			element->changeShapeType(shape);

			if (SettingsManager::value("ActivateGrid").toBool()) {
				element->alignToGrid();
			}
		}
	}
}

void EditorViewScene::initNodes()
{
	for (QGraphicsItem *item : items()) {
		NodeElement* node = dynamic_cast<NodeElement*>(item);
		if (node) {
			node->adjustLinks();
			if (mModels.graphicalModelAssistApi().properties(node->id()).contains("expanded")
					&& mModels.graphicalRepoApi().property(
							node->id(), "expanded").toString() == "true") {
				node->changeExpanded();
			}

			if (mModels.graphicalModelAssistApi().properties(node->id()).contains("folded")
					&& mModels.graphicalRepoApi().property(
							node->id(), "folded").toString() == "true") {
				node->changeFoldState();
			}
		}
	}
}

QAction &EditorViewScene::deleteAction()
{
	return mActionDeleteFromDiagram;
}

QList<QAction *> const &EditorViewScene::editorActions() const
{
	return mEditorActions;
}

void EditorViewScene::setActionsEnabled(bool enabled)
{
	for (QAction * const action : mEditorActions) {
		action->setEnabled(enabled);
	}

	mActionDeleteFromDiagram.setEnabled(enabled);
}

void EditorViewScene::onElementDeleted(Element *element)
{
	/// @todo: Make it more automated, conceptually this method is not needed.
	mHighlightedElements.remove(element);
}

void EditorViewScene::enableMouseGestures(bool enabled)
{
	mMouseGesturesEnabled = enabled;
	if (enabled) {
		mMouseMovementManager.reset(new gestures::MouseMovementManager(mRootId, mEditorManager));
	} else {
		mMouseMovementManager.reset(new gestures::DummyMouseMovementManager(mRootId, mEditorManager));
	}
}

void EditorViewScene::deselectLabels()
{
	for (QGraphicsItem * const item : items()) {
		Label * const label = dynamic_cast<Label *>(item);
		if (label && !label->isSelected()) {
			label->clearMoveFlag();
		}
	}
}
