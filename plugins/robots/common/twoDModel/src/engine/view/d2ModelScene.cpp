#include "d2ModelScene.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

#include <qrkernel/settingsManager.h>
#include <qrutils/graphicsUtils/gridDrawer.h>
#include <kitBase/readOnly.h>

#include "robotItem.h"

#include "src/engine/model/model.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/stylusItem.h"
#include "src/engine/items/ellipseItem.h"

using namespace twoDModel;
using namespace view;
using namespace qReal;
using namespace graphicsUtils;

D2ModelScene::D2ModelScene(model::Model &model
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

	connect(&mModel.worldModel(), &model::WorldModel::wallAdded, this, &D2ModelScene::onWallAdded);
	connect(&mModel.worldModel(), &model::WorldModel::colorItemAdded, this, &D2ModelScene::onColorItemAdded);
	connect(&mModel.worldModel(), &model::WorldModel::otherItemAdded, this, &D2ModelScene::onOtherItemAdded);
	connect(&mModel.worldModel(), &model::WorldModel::itemRemoved, [](QGraphicsItem *item) { delete item; });

	connect(&mModel, &model::Model::robotAdded, this, &D2ModelScene::onRobotAdd);
	connect(&mModel, &model::Model::robotRemoved, this, &D2ModelScene::onRobotRemove);
}

D2ModelScene::~D2ModelScene()
{
}

bool D2ModelScene::oneRobot() const
{
	return mRobots.size() == 1;
}

void D2ModelScene::setInteractivityFlags(int flags)
{
	mWorldReadOnly = (flags & kitBase::ReadOnly::World) != 0;
	mRobotReadOnly = (flags & kitBase::ReadOnly::RobotPosition) != 0;
	mSensorsReadOnly = (flags & kitBase::ReadOnly::Sensors) != 0;

	for (auto item : items()) {
		auto robotItem = dynamic_cast<RobotItem *>(item);
		auto sensorItem = dynamic_cast<SensorItem *>(item);
		auto worldItem = dynamic_cast<items::ColorFieldItem *>(item);
		if (worldItem) {
			worldItem->setEditable(!mWorldReadOnly);
		} else if (robotItem) {
			robotItem->setEditable(!mRobotReadOnly);
		} else if (sensorItem) {
			sensorItem->setEditable(!mSensorsReadOnly);
		}
	}
}

void D2ModelScene::handleNewRobotPosition(RobotItem *robotItem)
{
	for (const items::WallItem *wall : mModel.worldModel().walls()) {
		if (wall->realShape().intersects(robotItem->realBoundingRect())) {
			robotItem->recoverDragStartPosition();
			return;
		}
	}
}

void D2ModelScene::onRobotAdd(model::RobotModel *robotModel)
{
	RobotItem * const robotItem = new RobotItem(robotModel->info().robotImage(), *robotModel, this);

	connect(robotItem, &RobotItem::changedPosition, this, &D2ModelScene::handleNewRobotPosition);
	connect(robotItem, &RobotItem::mousePressed, this, &D2ModelScene::robotPressed);
	connect(robotItem, &RobotItem::drawTrace, &mModel.worldModel(), &model::WorldModel::appendRobotTrace);

	robotItem->setEditable(!mRobotReadOnly);

	addItem(robotItem);

	Rotater * const rotater = new Rotater();
	rotater->setMasterItem(robotItem);
	rotater->setVisible(false);

	robotItem->setRotater(rotater);

	mRobots.insert(robotModel, robotItem);

	emit robotListChanged(robotItem);
}

void D2ModelScene::onRobotRemove(model::RobotModel *robotModel)
{
	RobotItem *robotItem = mRobots[robotModel];

	removeItem(robotItem);
	mRobots.remove(robotModel);

	delete robotItem;

	emit robotListChanged(nullptr);
}

void D2ModelScene::onWallAdded(items::WallItem *wall)
{
	addItem(wall);
	connect(wall, &items::WallItem::wallDragged, this, &D2ModelScene::worldWallDragged);
	wall->setEditable(!mWorldReadOnly);
}

void D2ModelScene::onColorItemAdded(graphicsUtils::AbstractItem *item)
{
	addItem(item);
	item->setEditable(!mWorldReadOnly);
}

void D2ModelScene::onOtherItemAdded(QGraphicsItem *item)
{
	addItem(item);
}

void D2ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	const QPointF position = mouseEvent->scenePos();

	for (RobotItem * const robotItem : mRobots.values()) {
		robotItem->checkSelection();

		for (SensorItem *sensor : robotItem->sensors().values()) {
			if (sensor) {
				sensor->checkSelection();
			}
		}
	}

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
			case stylus:
				mCurrentStylus = new items::StylusItem(position.x(), position.y());
				initColorField(mCurrentStylus);
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

void D2ModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->buttons() & Qt::LeftButton) {
		for (RobotItem * const robotItem : mRobots.values()) {
			robotItem->checkSelection();
			for (SensorItem *sensor : robotItem->sensors().values()) {
				if (sensor) {
					sensor->checkSelection();
				}
			}
		}
	}

	bool needUpdate = true;
	switch (mDrawingAction){
	case wall:
		reshapeWall(mouseEvent);
		break;
	case line:
		reshapeLine(mouseEvent);
		break;
	case stylus:
		reshapeStylus(mouseEvent);
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

void D2ModelScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	for (RobotItem * const robotItem : mRobots.values()) {
		robotItem->checkSelection();
		for (SensorItem *sensor : robotItem->sensors().values()) {
			if (sensor) {
				sensor->checkSelection();
			}
		}
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
	case stylus: {
		reshapeStylus(mouseEvent);
		itemToSelect = mCurrentStylus;
		mCurrentStylus = nullptr;
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

void D2ModelScene::forPressResize(QGraphicsSceneMouseEvent *event)
{
	setX1andY1(event);
	mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(event->scenePos(), QTransform()));
	if (mGraphicsItem && mGraphicsItem->editable()) {
		mGraphicsItem->changeDragState(mX1, mY1);
		if (mGraphicsItem->getDragState() != AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}
	}

	update();
}

void D2ModelScene::deleteItem(QGraphicsItem *item)
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
	} else if (items::ColorFieldItem *colorField = dynamic_cast<items::ColorFieldItem *>(item)) {
		mModel.worldModel().removeColorField(colorField);
	}
}

void D2ModelScene::forMoveResize(QGraphicsSceneMouseEvent *event)
{
	reshapeItem(event);
	update();
}

void D2ModelScene::forReleaseResize(QGraphicsSceneMouseEvent * event)
{
	reshapeItem(event);
	mGraphicsItem = nullptr;
	update();
}

void D2ModelScene::reshapeItem(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	if (mGraphicsItem && mGraphicsItem->editable()) {
		const QPointF oldBegin = mGraphicsItem->getX1andY1();
		const QPointF oldEnd = mGraphicsItem->getX2andY2();
		if (mGraphicsItem->getDragState() != graphicsUtils::AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}

		mGraphicsItem->resizeItem(event);

		QPainterPath shape;

		for (RobotItem * const robotItem : mRobots.values()) {
			shape.addRect(robotItem->realBoundingRect());
		}

		if (mGraphicsItem->realShape().intersects(shape) && dynamic_cast<items::WallItem *>(mGraphicsItem)) {
			mGraphicsItem->reverseOldResizingItem(oldBegin, oldEnd);
		}
	}
}

void D2ModelScene::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && (selectedItems().size() > 0)) {
		for (QGraphicsItem * const item : selectedItems()) {
			auto worldItem = dynamic_cast<items::ColorFieldItem *>(item);
			auto robotItem = dynamic_cast<RobotItem *>(item);
			auto sensorItem = dynamic_cast<SensorItem *>(item);
			if (worldItem && mWorldReadOnly) {
				return;
			} else if (robotItem && mRobotReadOnly) {
				return;
			} else if (sensorItem && mSensorsReadOnly) {
				return;
			}

			deleteItem(item);
		}
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

void D2ModelScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	if (SettingsManager::value("2dShowGrid").toBool()) {
		mWidthOfGrid = SettingsManager::value("GridWidth").toReal() / 100;
		painter->setPen(QPen(Qt::black, mWidthOfGrid));
		QGraphicsScene::drawBackground(painter, rect);
		const int cellSize = SettingsManager::value("2dGridCellSize").toInt();
		mGridDrawer.drawGrid(painter, rect, cellSize);
	}
}

void D2ModelScene::addWall()
{
	mDrawingAction = wall;
}

void D2ModelScene::addLine()
{
	mDrawingAction = line;
}

void D2ModelScene::addStylus()
{
	mDrawingAction = stylus;
}

void D2ModelScene::addEllipse()
{
	mDrawingAction = ellipse;
}

void D2ModelScene::setNoneStatus()
{
	mDrawingAction = none;
}

void D2ModelScene::clearScene(bool removeRobot, Reason reason)
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

void D2ModelScene::reshapeWall(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentWall) {
		const QPointF oldPos = mCurrentWall->end();
		mCurrentWall->setX2andY2(pos.x(), pos.y());
		if (SettingsManager::value("2dShowGrid").toBool()) {
			mCurrentWall->reshapeBeginWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			mCurrentWall->reshapeEndWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		} else {
			const QPainterPath shape = mCurrentWall->realShape();

			for (RobotItem * const robotItem : mRobots.values()) {
				if (shape.intersects(robotItem->realBoundingRect())) {
					mCurrentWall->setX2andY2(oldPos.x(), oldPos.y());
					break;
				}
			}

			if (event->modifiers() & Qt::ShiftModifier) {
				mCurrentWall->reshapeRectWithShift();
			}
		}
	}
}

void D2ModelScene::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentLine) {
		mCurrentLine->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentLine->reshapeRectWithShift();
		}
	}
}

void D2ModelScene::reshapeStylus(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentStylus) {
		mCurrentStylus->addLine(pos.x(), pos.y());
	}
}

void D2ModelScene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	const QPointF pos = event->scenePos();
	if (mCurrentEllipse) {
		mCurrentEllipse->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentEllipse->reshapeRectWithShift();
		}
	}
}

void D2ModelScene::worldWallDragged(items::WallItem *wall, const QPainterPath &shape, const QRectF &oldPos)
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
			(mDrawingAction == D2ModelScene::wall && mCurrentWall == wall)))
	{
		wall->setFlag(QGraphicsItem::ItemIsMovable, !isNeedStop);
		if (isNeedStop) {
			wall->setCoordinates(oldPos);
		}
	}
}

void D2ModelScene::alignWalls()
{
	if (SettingsManager::value("2dShowGrid").toBool()) {
		for (items::WallItem * const wall : mModel.worldModel().walls()) {
			if (items().contains(wall)) {
				wall->alignTheWall(SettingsManager::value("2dGridCellSize").toInt());
			}
		}
	}
}

RobotItem *D2ModelScene::robot(model::RobotModel &robotModel)
{
	return mRobots.value(&robotModel);
}

void D2ModelScene::centerOnRobot(RobotItem *selectedItem)
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
