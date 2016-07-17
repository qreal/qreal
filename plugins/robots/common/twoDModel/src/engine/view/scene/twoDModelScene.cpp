/* Copyright 2012-2016 CyberTech Labs Ltd.
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
#include <qrgui/controller/controllerInterface.h>

#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/colorSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <kitBase/robotModel/robotParts/vectorSensor.h>

#include "robotItem.h"

#include "twoDModel/engine/model/model.h"
#include "src/engine/view/scene/sensorItem.h"
#include "src/engine/view/scene/sonarSensorItem.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/curveItem.h"
#include "src/engine/items/stylusItem.h"
#include "src/engine/items/rectangleItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/regions/regionItem.h"
#include "src/engine/items/startPosition.h"

#include "src/engine/commands/createWorldItemCommand.h"
#include "src/engine/commands/removeWorldItemsCommand.h"
#include "src/engine/commands/removeSensorCommand.h"
#include "src/engine/commands/reshapeCommand.h"

using namespace twoDModel;
using namespace view;
using namespace qReal;
using namespace graphicsUtils;

const QString twoDModelUndoStackName = "2D-model-undo-stack";

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

void TwoDModelScene::setController(ControllerInterface &controller)
{
	mController = &controller;
	mController->moduleOpened(twoDModelUndoStackName);
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
	subscribeItem(static_cast<AbstractItem *>(robotModel->startPositionMarker()));

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
	subscribeItem(wall);
	connect(wall, &items::WallItem::wallDragged, this, &TwoDModelScene::worldWallDragged);
	connect(wall, &items::WallItem::deletedWithContextMenu, this, &TwoDModelScene::deleteSelectedItems);
	wall->setEditable(!mWorldReadOnly);
}

void TwoDModelScene::onColorItemAdded(graphicsUtils::AbstractItem *item)
{
	addItem(item);
	subscribeItem(item);
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

void TwoDModelScene::drawAxes(QPainter *painter)
{
	painter->save();

	const int arrowSize = 5;
	const QRectF visibleRect = views().first()->mapToScene(views().first()->viewport()->geometry()).boundingRect();

	QPen pen = painter->pen();
	pen.setColor(Qt::gray);
	pen.setWidth(2);
	pen.setStyle(Qt::DashLine);
	painter->setPen(pen);

	painter->drawLine(0, visibleRect.top(), 0, visibleRect.bottom());
	painter->drawLine(0, visibleRect.bottom(), -arrowSize, visibleRect.bottom() - arrowSize);
	painter->drawLine(0, visibleRect.bottom(), arrowSize, visibleRect.bottom() - arrowSize);
	painter->drawLine(visibleRect.left(), 0, visibleRect.right(), 0);
	painter->drawLine(visibleRect.right(), 0, visibleRect.right() - arrowSize, -arrowSize);
	painter->drawLine(visibleRect.right(), 0, visibleRect.right() - arrowSize, arrowSize);

	painter->restore();
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
	AbstractItem *createdItem = nullptr;
	switch (mDrawingAction){
	case wall: {
		reshapeWall(mouseEvent);
		createdItem = mCurrentWall;
		mCurrentWall = nullptr;
		break;
	}
	case line: {
		reshapeLine(mouseEvent);
		createdItem = mCurrentLine;
		mCurrentLine = nullptr;
		break;
	}
	case bezier: {
		reshapeCurve(mouseEvent);
		createdItem = mCurrentCurve;
		mCurrentCurve = nullptr;
		break;
	}
	case stylus: {
		reshapeStylus(mouseEvent);
		createdItem = mCurrentStylus;
		mCurrentStylus = nullptr;
		break;
	}
	case rectangle: {
		reshapeRectangle(mouseEvent);
		createdItem = mCurrentRectangle;
		mCurrentRectangle = nullptr;
		break;
	}
	case ellipse: {
		reshapeEllipse(mouseEvent);
		createdItem = mCurrentEllipse;
		mCurrentEllipse = nullptr;
		break;
	}
	default:
		if (createdItem) {
			forReleaseResize(mouseEvent);
		}
		break;
	}

	if (createdItem) {
		createdItem->setSelected(true);
		if (mDrawingAction != none && mController) {
			commands::CreateWorldItemCommand *command = new commands::CreateWorldItemCommand(mModel, createdItem->id());
			// Command was already executed when element was drawn by user. So we should create it in redone state.
			command->setRedoEnabled(false);
			mController->execute(command);
			command->setRedoEnabled(true);
		}
	}

	setMoveFlag(mouseEvent);

	for (RobotItem * const robotItem : mRobots) {
		setSceneRect(sceneRect().united(robotItem->sceneBoundingRect()));
	}

	update();
	AbstractScene::mouseReleaseEvent(mouseEvent);
}

void TwoDModelScene::deleteSelectedItems()
{
	QStringList worldItemsToDelete;
	QList<QPair<model::RobotModel *, kitBase::robotModel::PortInfo>> sensorsToDelete;
	for (QGraphicsItem * const item : selectedItems()) {
		SensorItem * const sensor = dynamic_cast<SensorItem *>(item);
		items::WallItem * const wall = dynamic_cast<items::WallItem *>(item);
		items::ColorFieldItem * const colorField = dynamic_cast<items::ColorFieldItem *>(item);

		if (sensor && !mSensorsReadOnly) {
			for (RobotItem * const robotItem : mRobots.values()) {
				const kitBase::robotModel::PortInfo port = robotItem->sensors().key(sensor);
				if (port.isValid()) {
					sensorsToDelete << qMakePair(&robotItem->robotModel(), port);
				}
			}
		} else if (wall && !mWorldReadOnly) {
			worldItemsToDelete << wall->id();
			mCurrentWall = nullptr;
		} else if (colorField && !mWorldReadOnly) {
			worldItemsToDelete << colorField->id();
			mCurrentLine = nullptr;
			mCurrentStylus = nullptr;
			mCurrentEllipse = nullptr;
			mCurrentRectangle = nullptr;
			mCurrentCurve = nullptr;
		}
	}

	deleteWithCommand(worldItemsToDelete, sensorsToDelete, {});
}

void TwoDModelScene::deleteWithCommand(const QStringList &worldItems
		, const QList<QPair<model::RobotModel *, kitBase::robotModel::PortInfo>> &sensors
		, const QList<qReal::commands::AbstractCommand *> &additionalCommands)
{
	const bool shouldCreateCommand = !worldItems.isEmpty() || !sensors.isEmpty();
	if (mController && shouldCreateCommand) {
		auto command = new commands::RemoveWorldItemsCommand(mModel, worldItems);
		// Appending sensors deletion commands
		for (const QPair<model::RobotModel *, kitBase::robotModel::PortInfo> &sensor : sensors) {
			command->addPostAction(new commands::RemoveSensorCommand(sensor.first->configuration()
					, sensor.first->info().robotId(), sensor.second));
		}

		for (qReal::commands::AbstractCommand * const additionalCommand : additionalCommands) {
			command->addPostAction(additionalCommand);
		}

		mController->execute(command);
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
	if (event->key() == Qt::Key_Delete) {
		deleteSelectedItems();
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

void TwoDModelScene::focusInEvent(QFocusEvent *event)
{
	QGraphicsScene::focusInEvent(event);
	if (mController) {
		mController->setActiveModule(twoDModelUndoStackName);
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
		drawAxes(painter);
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
	if (reason == Reason::userAction) {
		// User pressed clear button, this action must be undone when required, so executing it with command.

		QStringList worldItemsToDelete;
		for (const items::WallItem *wall : mModel.worldModel().walls()) {
			worldItemsToDelete << wall->id();
		}
		for (const items::ColorFieldItem *colorField : mModel.worldModel().colorFields()) {
			worldItemsToDelete << colorField->id();
		}
		for (const items::RegionItem *region : mModel.worldModel().regions()) {
			worldItemsToDelete << region->id();
		}

		QList<QPair<model::RobotModel *, kitBase::robotModel::PortInfo>> sensorsToDelete;
		QList<qReal::commands::AbstractCommand *> additionalCommands;
		for (model::RobotModel *robotModel : mRobots.keys()) {
			commands::ReshapeCommand * const reshapeCommand = new commands::ReshapeCommand(*this, mModel
					, {mRobots[robotModel]->id()});
			reshapeCommand->startTracking();
			robotModel->clear();
			reshapeCommand->stopTracking();
			additionalCommands << reshapeCommand;
			if (removeRobot) {
				for (const kitBase::robotModel::PortInfo &port : robot(*robotModel)->sensors().keys()) {
					sensorsToDelete << qMakePair(robotModel, port);
				}
			}
		}

		deleteWithCommand(worldItemsToDelete, sensorsToDelete, additionalCommands);

		// Clear trace action mustn`t be undone though
		mModel.worldModel().clearRobotTrace();

	} else {
		// The model is being reloaded, we can make it without undo-redo, so a bit faster

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

void TwoDModelScene::subscribeItem(AbstractItem *item)
{
	connect(item, &AbstractItem::mouseInteractionStarted, this, [=]() {
		if (mDrawingAction == none) {
			QStringList selectedIds;
			for (const QGraphicsItem *item : selectedItems()) {
				if (const AbstractItem *itemWithId = dynamic_cast<const AbstractItem *>(item)) {
					selectedIds << itemWithId->id();
				}
			}

			mCurrentReshapeCommand = new commands::ReshapeCommand(*this, mModel, selectedIds);
			mCurrentReshapeCommand->startTracking();
		}
	});

	connect(item, &AbstractItem::mouseInteractionStopped, this, [=]() {
		if (mDrawingAction == none) {
			mCurrentReshapeCommand->stopTracking();
			if (mController) {
				mController->execute(mCurrentReshapeCommand);
			}

			mCurrentReshapeCommand = nullptr;
		}
	});
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

void TwoDModelScene::reinitSensor(RobotItem *robotItem, const kitBase::robotModel::PortInfo &port)
{
	robotItem->removeSensor(port);
	model::RobotModel &robotModel = robotItem->robotModel();

	const kitBase::robotModel::DeviceInfo &device = robotModel.configuration().type(port);
	if (device.isNull() || (
			/// @todo: Add supported by 2D model sensors here
			!device.isA<kitBase::robotModel::robotParts::TouchSensor>()
			&& !device.isA<kitBase::robotModel::robotParts::ColorSensor>()
			&& !device.isA<kitBase::robotModel::robotParts::LightSensor>()
			&& !device.isA<kitBase::robotModel::robotParts::RangeSensor>()
			/// @todo For working with line sensor from TRIK. Actually this information shall be loaded from plugins.
			&& !device.isA<kitBase::robotModel::robotParts::VectorSensor>()
			))
	{
		return;
	}

	SensorItem *sensor = device.isA<kitBase::robotModel::robotParts::RangeSensor>()
			? new SonarSensorItem(mModel.worldModel(), robotModel.configuration()
					, port
					, robotModel.info().sensorImagePath(device)
					, robotModel.info().sensorImageRect(device)
					)
			: new SensorItem(robotModel.configuration()
					, port
					, robotModel.info().sensorImagePath(device)
					, robotModel.info().sensorImageRect(device)
					);

	sensor->setEditable(!mSensorsReadOnly);
	subscribeItem(sensor);

	robotItem->addSensor(port, sensor);
}
