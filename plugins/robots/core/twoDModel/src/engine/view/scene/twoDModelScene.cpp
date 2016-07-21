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

#include "twoDModelScene.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

#include <qrkernel/settingsManager.h>
#include <qrutils/graphicsUtils/gridDrawer.h>
#include <qrutils/deleteLaterHelper.h>

#include "robotItem.h"

#include "twoDModel/engine/model/model.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/curveItem.h"
#include "src/engine/items/stylusItem.h"
#include "src/engine/items/rectangleItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/regions/regionItem.h"
#include "src/engine/items/startPosition.h"

using namespace twoDModel;
using namespace view;
using namespace qReal;
using namespace graphicsUtils;

TwoDModelScene::TwoDModelScene(model::Model &model
		, AbstractView *view
		, QObject *parent)
	: AbstractScene(view, parent)
	, mModel(model)
	, mDrawingAction(none)
{
	mFirstPenWidth = 6;
	mSizeEmptyRectX = 1000;
	mSizeEmptyRectY = 1000;
	setEmptyRect(-500, -500, mSizeEmptyRectX, mSizeEmptyRectY);
	setItemIndexMethod(NoIndex);
	setEmptyPenBrushItems();

	connect(&mModel.worldModel(), &model::WorldModel::wallAdded, this, &TwoDModelScene::onWallAdded);
	connect(&mModel.worldModel(), &model::WorldModel::colorItemAdded, this, &TwoDModelScene::onColorItemAdded);
	connect(&mModel.worldModel(), &model::WorldModel::regionItemAdded, [=](items::RegionItem *item) { addItem(item); });
	connect(&mModel.worldModel(), &model::WorldModel::traceItemAdded, [=](QGraphicsLineItem *item) { addItem(item); });
	connect(&mModel.worldModel(), &model::WorldModel::itemRemoved, this, &TwoDModelScene::onItemRemoved);

	connect(&mModel, &model::Model::robotAdded, this, &TwoDModelScene::onRobotAdd);
	connect(&mModel, &model::Model::robotRemoved, this, &TwoDModelScene::onRobotRemove);
}

TwoDModelScene::~TwoDModelScene()
{
}

bool TwoDModelScene::oneRobot() const
{
	return mRobots.size() == 1;
}

void TwoDModelScene::setInteractivityFlags(kitBase::ReadOnlyFlags flags)
{
	mWorldReadOnly = (flags & kitBase::ReadOnly::World) != 0;
	mRobotReadOnly = (flags & kitBase::ReadOnly::RobotPosition) != 0;
	mSensorsReadOnly = (flags & kitBase::ReadOnly::Sensors) != 0;

	for (const auto item : items()) {
		const auto robotItem = dynamic_cast<RobotItem *>(item);
		const auto sensorItem = dynamic_cast<SensorItem *>(item);
		const auto worldItem = dynamic_cast<items::ColorFieldItem *>(item);
		const auto startPosition = dynamic_cast<items::StartPosition *>(item);
		if (worldItem) {
			worldItem->setEditable(!mWorldReadOnly);
		} else if (robotItem) {
			robotItem->setEditable(!mRobotReadOnly);
		} else if (sensorItem) {
			sensorItem->setEditable(!mSensorsReadOnly);
		} else if (startPosition) {
			startPosition->setEditable(!mRobotReadOnly);
		}
	}
}

void TwoDModelScene::handleNewRobotPosition(RobotItem *robotItem)
{
	for (const items::WallItem *wall : mModel.worldModel().walls()) {
		if (wall->realShape().intersects(robotItem->realBoundingRect())) {
			robotItem->recoverDragStartPosition();
			return;
		}
	}
}

void TwoDModelScene::onRobotAdd(model::RobotModel *robotModel)
{
	RobotItem * const robotItem = new RobotItem(robotModel->info().robotImage(), *robotModel);

	connect(robotItem, &RobotItem::changedPosition, this, &TwoDModelScene::handleNewRobotPosition);
	connect(robotItem, &RobotItem::mousePressed, this, &TwoDModelScene::robotPressed);
	connect(robotItem, &RobotItem::drawTrace, &mModel.worldModel(), &model::WorldModel::appendRobotTrace);

	robotItem->setEditable(!mRobotReadOnly);

	addItem(robotItem);
	addItem(robotItem->robotModel().startPositionMarker());

	mRobots.insert(robotModel, robotItem);

	emit robotListChanged(robotItem);
}

void TwoDModelScene::onRobotRemove(model::RobotModel *robotModel)
{
	RobotItem *robotItem = mRobots[robotModel];

	removeItem(robotItem);
	mRobots.remove(robotModel);

	delete robotItem;

	emit robotListChanged(nullptr);
}

void TwoDModelScene::onWallAdded(items::WallItem *wall)
{
	addItem(wall);
	connect(wall, &items::WallItem::wallDragged, this, &TwoDModelScene::worldWallDragged);
	connect(wall, &items::WallItem::deletedWithContextMenu, this, &TwoDModelScene::deleteSelectedItems);
	wall->setEditable(!mWorldReadOnly);
}

void TwoDModelScene::onColorItemAdded(graphicsUtils::AbstractItem *item)
{
	addItem(item);
	connect(item, &graphicsUtils::AbstractItem::deletedWithContextMenu, this, &TwoDModelScene::deleteSelectedItems);
	item->setEditable(!mWorldReadOnly);
}

void TwoDModelScene::onItemRemoved(QGraphicsItem *item)
{
	mGraphicsItem = nullptr;
	removeItem(item);
	// We delete the item not immediately cause in other handlers of WorldModel`s itemRemoved() signal
	// it may still be used.
	utils::DeleteLaterHelper<QGraphicsItem>::deleteLater(item);
}

void TwoDModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton) {
		AbstractScene::mousePressEvent(mouseEvent);
		return;
	}

	const QPointF position = mouseEvent->scenePos();

	emit mousePressed();

	auto initItem = [this, mouseEvent](QGraphicsItem *item) {
		removeMoveFlag(mouseEvent, item);
		// This will deselect all items
		setSelectionArea(QPainterPath());
	};

	auto initColorField = [this, mouseEvent, &initItem](items::ColorFieldItem *item) {
		initItem(item);
		item->setPenBrush(penStyleItems(), penWidthItems(), penColorItems()
				, brushStyleItems(), brushColorItems());
		mModel.worldModel().addColorField(item);
	};

	for (RobotItem * const robotItem : mRobots.values()) {
		if (!robotItem->realBoundingRect().contains(position)) {
			switch (mDrawingAction) {
			case wall:
				mCurrentWall = new items::WallItem(position, position);
				initItem(mCurrentWall);
				mModel.worldModel().addWall(mCurrentWall);
				break;
			case line:
				mCurrentLine = new items::LineItem(position, position);
				initColorField(mCurrentLine);
				break;
			case bezier:
				mCurrentCurve = new items::CurveItem(position, position);
				initColorField(mCurrentCurve);
				break;
			case stylus:
				mCurrentStylus = new items::StylusItem(position.x(), position.y());
				initColorField(mCurrentStylus);
				break;
			case rectangle:
				mCurrentRectangle = new items::RectangleItem(position, position);
				initColorField(mCurrentRectangle);
				break;
			case ellipse:
				mCurrentEllipse = new items::EllipseItem(position, position);
				initColorField(mCurrentEllipse);
				break;
			default:
				break;
			}
		}
	}

	if (mDrawingAction == none) {
		forPressResize(mouseEvent);
	}

	AbstractScene::mousePressEvent(mouseEvent);
}

void TwoDModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	bool needUpdate = true;
	switch (mDrawingAction) {
	case wall:
		reshapeWall(mouseEvent);
		break;
	case line:
		reshapeLine(mouseEvent);
		break;
	case bezier:
		reshapeCurve(mouseEvent);
		break;
	case stylus:
		reshapeStylus(mouseEvent);
		break;
	case rectangle:
		reshapeRectangle(mouseEvent);
		break;
	case ellipse:
		reshapeEllipse(mouseEvent);
		break;
	default:
		needUpdate = false;
		if (mouseEvent->buttons() & Qt::LeftButton) {
			forMoveResize(mouseEvent);
		}

		AbstractScene::mouseMoveEvent(mouseEvent);
		break;
	}

	if (needUpdate) {
		update();
	}
}

void TwoDModelScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton) {
		AbstractScene::mouseReleaseEvent(mouseEvent);
		return;
	}

	emit mouseReleased();

	// After dragging item may be null. We mustn`t select it in that case.
	QGraphicsItem *itemToSelect = nullptr;
	switch (mDrawingAction){
	case wall: {
		reshapeWall(mouseEvent);
		itemToSelect = mCurrentWall;
		mCurrentWall = nullptr;
		break;
	}
	case line: {
		reshapeLine(mouseEvent);
		itemToSelect = mCurrentLine;
		mCurrentLine = nullptr;
		break;
	}
	case bezier: {
		reshapeCurve(mouseEvent);
		itemToSelect = mCurrentCurve;
		mCurrentCurve = nullptr;
		break;
	}
	case stylus: {
		reshapeStylus(mouseEvent);
		itemToSelect = mCurrentStylus;
		mCurrentStylus = nullptr;
		break;
	}
	case rectangle: {
		reshapeRectangle(mouseEvent);
		itemToSelect = mCurrentRectangle;
		mCurrentRectangle = nullptr;
		break;
	}
	case ellipse: {
		reshapeEllipse(mouseEvent);
		itemToSelect = mCurrentEllipse;
		mCurrentEllipse = nullptr;
		break;
	}
	default:
		if (itemToSelect) {
			forReleaseResize(mouseEvent);
		}
		break;
	}

	if (itemToSelect) {
		itemToSelect->setSelected(true);
	}

	setMoveFlag(mouseEvent);

	for (RobotItem * const robotItem : mRobots) {
		setSceneRect(sceneRect().united(robotItem->sceneBoundingRect()));
	}

	update();
	AbstractScene::mouseReleaseEvent(mouseEvent);
}

void TwoDModelScene::deleteItem(QGraphicsItem *item)
{
	if (!items().contains(item)) {
		return;
	}

	if (SensorItem * const sensor = dynamic_cast<SensorItem *>(item)) {
		for (RobotItem * const robotItem : mRobots.values()) {
			const kitBase::robotModel::PortInfo port = robotItem->sensors().key(sensor);
			if (port.isValid()) {
				deviceConfigurationChanged(robotItem->robotModel().info().robotId()
						, port, kitBase::robotModel::DeviceInfo(), Reason::userAction);
			}
		}
	} else if (items::WallItem * const wall = dynamic_cast<items::WallItem *>(item)) {
		mModel.worldModel().removeWall(wall);
		mCurrentWall = nullptr;
	} else if (items::ColorFieldItem *colorField = dynamic_cast<items::ColorFieldItem *>(item)) {
		mModel.worldModel().removeColorField(colorField);
		mCurrentLine = nullptr;
		mCurrentStylus = nullptr;
		mCurrentEllipse = nullptr;
		mCurrentRectangle = nullptr;
		mCurrentCurve = nullptr;
	}
}

void TwoDModelScene::deleteSelectedItems()
{
	for (QGraphicsItem * const item : selectedItems()) {
		deleteItem(item);
	}
}

void TwoDModelScene::reshapeItem(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	if (mGraphicsItem && mGraphicsItem->editable()) {
		const QPointF oldBegin(mGraphicsItem->x1(), mGraphicsItem->y1());
		const QPointF oldEnd(mGraphicsItem->x2(), mGraphicsItem->y2());
		if (mGraphicsItem->dragState() != graphicsUtils::AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}

		mGraphicsItem->resizeItem(event);

		QPainterPath shape;

		for (RobotItem * const robotItem : mRobots.values()) {
			shape.addRect(robotItem->realBoundingRect());
		}

		if (dynamic_cast<items::WallItem *>(mGraphicsItem) && mGraphicsItem->realShape().intersects(shape)) {
			mGraphicsItem->reverseOldResizingItem(oldBegin, oldEnd);
		}
	}
}

void TwoDModelScene::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && (selectedItems().size() > 0)) {
		for (QGraphicsItem * const item : selectedItems()) {
			const bool isWorldItem = dynamic_cast<items::ColorFieldItem *>(item)
					|| dynamic_cast<items::WallItem *>(item);
			const bool isRobotItem = dynamic_cast<RobotItem *>(item) != nullptr;
			const bool isSensorItem = dynamic_cast<SensorItem *>(item) != nullptr;
			if (isWorldItem && mWorldReadOnly) {
				return;
			} else if (isRobotItem && mRobotReadOnly) {
				return;
			} else if (isSensorItem && mSensorsReadOnly) {
				return;
			}

			deleteItem(item);
		}
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

void TwoDModelScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (SettingsManager::value("2dShowGrid").toBool()) {
		mWidthOfGrid = SettingsManager::value("GridWidth").toReal() / 100;
		painter->setPen(QPen(Qt::black, mWidthOfGrid));
		QGraphicsScene::drawBackground(painter, rect);
		const int cellSize = SettingsManager::value("2dGridCellSize").toInt();
		mGridDrawer.drawGrid(painter, rect, cellSize);
	}
}

void TwoDModelScene::addWall()
{
	mDrawingAction = wall;
}

void TwoDModelScene::addLine()
{
	mDrawingAction = line;
}

void TwoDModelScene::addBezier()
{
	mDrawingAction = bezier;
}

void TwoDModelScene::addStylus()
{
	mDrawingAction = stylus;
}

void TwoDModelScene::addRectangle()
{
	mDrawingAction = rectangle;
}

void TwoDModelScene::addEllipse()
{
	mDrawingAction = ellipse;
}

void TwoDModelScene::setNoneStatus()
{
	mDrawingAction = none;
}

void TwoDModelScene::clearScene(bool removeRobot, Reason reason)
{
	mModel.worldModel().clear();

	for (model::RobotModel *robotModel : mRobots.keys()) {
		robotModel->clear();
		if (removeRobot) {
			for (const kitBase::robotModel::PortInfo &port : robot(*robotModel)->sensors().keys()) {
				deviceConfigurationChanged(robotModel->info().robotId()
						, port, kitBase::robotModel::DeviceInfo(), reason);
			}
		}
	}
}

void TwoDModelScene::reshapeWall(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentWall) {
		const QPointF oldPos = mCurrentWall->end();
		mCurrentWall->setX2(pos.x());
		mCurrentWall->setY2(pos.y());
		if (SettingsManager::value("2dShowGrid").toBool()) {
			mCurrentWall->reshapeBeginWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			mCurrentWall->reshapeEndWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		} else {
			const QPainterPath shape = mCurrentWall->realShape();

			for (RobotItem * const robotItem : mRobots.values()) {
				if (shape.intersects(robotItem->realBoundingRect())) {
					mCurrentWall->setX2(oldPos.x());
					mCurrentWall->setY2(oldPos.y());
					break;
				}
			}

			if (event->modifiers() & Qt::ShiftModifier) {
				mCurrentWall->reshapeRectWithShift();
			}
		}
	}
}

void TwoDModelScene::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentLine) {
		mCurrentLine->setX2(pos.x());
		mCurrentLine->setY2(pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentLine->reshapeRectWithShift();
		}
	}
}

void TwoDModelScene::reshapeCurve(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentCurve) {
		mCurrentCurve->setX2(pos.x());
		mCurrentCurve->setY2(pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentCurve->reshapeRectWithShift();
		}
	}
}

void TwoDModelScene::reshapeStylus(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentStylus) {
		mCurrentStylus->addLine(pos.x(), pos.y());
	}
}

void TwoDModelScene::reshapeRectangle(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentRectangle) {
		mCurrentRectangle->setX2(pos.x());
		mCurrentRectangle->setY2(pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentRectangle->reshapeRectWithShift();
		}
	}
}

void TwoDModelScene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentEllipse) {
		mCurrentEllipse->setX2(pos.x());
		mCurrentEllipse->setY2(pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentEllipse->reshapeRectWithShift();
		}
	}
}

void TwoDModelScene::worldWallDragged(items::WallItem *wall, const QPainterPath &shape, const QRectF &oldPos)
{
	bool isNeedStop = false;

	for (RobotItem * const robotItem : mRobots.values()) {
		if (shape.intersects(robotItem->realBoundingRect())) {
			isNeedStop = true;
			break;
		}
	}

	wall->onOverlappedWithRobot(isNeedStop);
	if (wall->isDragged() && ((mDrawingAction == none) ||
			(mDrawingAction == TwoDModelScene::wall && mCurrentWall == wall)))
	{
		wall->setFlag(QGraphicsItem::ItemIsMovable, !isNeedStop);
		if (isNeedStop) {
			wall->setCoordinates(oldPos);
		}
	}
}

void TwoDModelScene::alignWalls()
{
	if (SettingsManager::value("2dShowGrid").toBool()) {
		for (items::WallItem * const wall : mModel.worldModel().walls()) {
			if (items().contains(wall)) {
				wall->alignTheWall(SettingsManager::value("2dGridCellSize").toInt());
			}
		}
	}
}

RobotItem *TwoDModelScene::robot(model::RobotModel &robotModel)
{
	return mRobots.value(&robotModel);
}

void TwoDModelScene::centerOnRobot(RobotItem *selectedItem)
{
	RobotItem *robotItem = mRobots.values().first();

	if (selectedItem) {
		robotItem = selectedItem;
	}

	for (QGraphicsView * const view : views()) {
		const QRectF viewPortRect = view->mapToScene(view->viewport()->rect()).boundingRect();
		if (!viewPortRect.contains(robotItem->sceneBoundingRect().toRect())) {
			const QRectF requiredViewPort = viewPortRect.translated(robotItem->scenePos() - viewPortRect.center());
			setSceneRect(itemsBoundingRect().united(requiredViewPort));
			view->centerOn(robotItem);
		}
	}
}
