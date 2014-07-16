#include "d2ModelScene.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

#include <qrkernel/settingsManager.h>
#include <qrutils/graphicsUtils/gridDrawer.h>

#include "robotItem.h"
#include "commonTwoDModel/engine/configurer.h"

#include "src/engine/model/model.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/stylusItem.h"
#include "src/engine/items/ellipseItem.h"

using namespace twoDModel;
using namespace view;
using namespace qReal;
using namespace graphicsUtils;

D2ModelScene::D2ModelScene(model::Model &model
		, Configurer const &configurer
		, AbstractView *view
		, QObject *parent)
	: AbstractScene(view, parent)
	, mModel(model)
	, mConfigurer(configurer)
	, mDrawingAction(none)
	, mRobot(nullptr)
	, mCurrentWall(nullptr)
	, mCurrentLine(nullptr)
	, mCurrentStylus(nullptr)
	, mCurrentEllipse(nullptr)
{
	mFirstPenWidth = 6;
	mSizeEmptyRectX = 1000;
	mSizeEmptyRectY = 1000;
	setEmptyRect(-500, -500, mSizeEmptyRectX, mSizeEmptyRectY);
	setItemIndexMethod(NoIndex);
	setEmptyPenBrushItems();

	connect(&mModel.worldModel(), &model::WorldModel::wallAdded, this, &QGraphicsScene::addItem);
	connect(&mModel.worldModel(), &model::WorldModel::wallAdded, [this](items::WallItem *wall) {
		connect(wall, &items::WallItem::wallDragged, this, &D2ModelScene::worldWallDragged);
	});
	connect(&mModel.worldModel(), &model::WorldModel::colorItemAdded, this, &QGraphicsScene::addItem);
	connect(&mModel.worldModel(), &model::WorldModel::itemRemoved, [](QGraphicsItem *item) { delete item; });

	drawInitialRobot();
}

D2ModelScene::~D2ModelScene()
{
	delete mRobot;
}

void D2ModelScene::drawInitialRobot()
{
	mRobot = new RobotItem(mConfigurer.robotImage(), mModel.robotModel());
	connect(mRobot, &RobotItem::changedPosition, this, &D2ModelScene::handleNewRobotPosition);
	connect(mRobot, &RobotItem::mousePressed, this, &D2ModelScene::robotPressed);
	addItem(mRobot);

	Rotater * const rotater = new Rotater();
	rotater->setMasterItem(mRobot);
	rotater->setVisible(false);

	mRobot->setRotater(rotater);

	centerOnRobot();
}

void D2ModelScene::handleNewRobotPosition()
{
	for (items::WallItem const *wall : mModel.worldModel().walls()) {
		if (wall->realShape().intersects(mRobot->realBoundingRect())) {
			mRobot->recoverDragStartPosition();
			return;
		}
	}
}

void D2ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF const position = mouseEvent->scenePos();

	mRobot->checkSelection();
	for (SensorItem *sensor : mRobot->sensors().values()) {
		if (sensor) {
			sensor->checkSelection();
		}
	}

	emit mousePressed();

	auto initItem = [this, mouseEvent](QGraphicsItem *item) {
		removeMoveFlag(mouseEvent, item);
		// This will deselect alll items
		setSelectionArea(QPainterPath());
	};

	auto initColorField = [this, mouseEvent, initItem](items::ColorFieldItem *item) {
		initItem(item);
		item->setPenBrush(penStyleItems(), penWidthItems(), penColorItems()
				, brushStyleItems(), brushColorItems());
		mModel.worldModel().addColorField(item);
	};

	if (!mRobot->realBoundingRect().contains(position)) {
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

	if (mDrawingAction == none) {
		forPressResize(mouseEvent);
	}

	AbstractScene::mousePressEvent(mouseEvent);
}

void D2ModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->buttons() & Qt::LeftButton) {
		mRobot->checkSelection();
		for (SensorItem *sensor : mRobot->sensors().values()) {
			if (sensor) {
				sensor->checkSelection();
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
			forMoveResize(mouseEvent, mRobot->realBoundingRect());
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
	mRobot->checkSelection();
	for (SensorItem *sensor : mRobot->sensors().values()) {
		if (sensor) {
			sensor->checkSelection();
		}
	}

	emit mouseReleased();

	switch (mDrawingAction){
	case wall: {
		reshapeWall(mouseEvent);
		mCurrentWall->setSelected(true);
		mCurrentWall = nullptr;
		break;
	}
	case line: {
		reshapeLine(mouseEvent);
		mCurrentLine->setSelected(true);
		mCurrentLine = nullptr;
		break;
	}
	case stylus: {
		reshapeStylus(mouseEvent);
		mCurrentStylus->setSelected(true);
		mCurrentStylus = nullptr;
		break;
	}
	case ellipse: {
		reshapeEllipse(mouseEvent);
		mCurrentEllipse->setSelected(true);
		mCurrentEllipse = nullptr;
		break;
	}
	default:
		forReleaseResize(mouseEvent, mRobot->realBoundingRect());
		break;
	}

	setMoveFlag(mouseEvent);

	setSceneRect(sceneRect().united(mRobot->sceneBoundingRect()));
	update();
	AbstractScene::mouseReleaseEvent(mouseEvent);
}

void D2ModelScene::forPressResize(QGraphicsSceneMouseEvent *event)
{
	setX1andY1(event);
	mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(event->scenePos(), QTransform()));
	if (mGraphicsItem) {
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
		interpreterBase::robotModel::PortInfo const port = mRobot->sensors().key(sensor);
		if (port.isValid()) {
			deviceConfigurationChanged(mModel.robotModel().info().name()
					, port, interpreterBase::robotModel::DeviceInfo());
		}
	} else if (items::WallItem * const wall = dynamic_cast<items::WallItem *>(item)) {
		mModel.worldModel().removeWall(wall);
	} else if (items::ColorFieldItem *colorField = dynamic_cast<items::ColorFieldItem *>(item)) {
		mModel.worldModel().removeColorField(colorField);
	}
}

void D2ModelScene::forMoveResize(QGraphicsSceneMouseEvent *event, QRectF const &rect)
{
	reshapeItem(event, rect);
	update();
}

void D2ModelScene::forReleaseResize(QGraphicsSceneMouseEvent * event, QRectF const &rect)
{
	reshapeItem(event, rect);
	mGraphicsItem = nullptr;
	update();
}

void D2ModelScene::reshapeItem(QGraphicsSceneMouseEvent *event, QRectF const &rect)
{
	setX2andY2(event);
	if (mGraphicsItem) {
		QPointF const oldBegin = mGraphicsItem->getX1andY1();
		QPointF const oldEnd = mGraphicsItem->getX2andY2();
		if (mGraphicsItem->getDragState() != graphicsUtils::AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}

		mGraphicsItem->resizeItem(event);

		if (mGraphicsItem->realShape().intersects(rect) && dynamic_cast<items::WallItem *>(mGraphicsItem)) {
			mGraphicsItem->reverseOldResizingItem(oldBegin, oldEnd);
		}
	}
}

void D2ModelScene::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && (selectedItems().size() > 0)) {
		for (QGraphicsItem * const item : selectedItems()) {
			deleteItem(item);
		}
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

void D2ModelScene::drawBackground(QPainter *painter, QRectF const &rect)
{
	if (SettingsManager::value("2dShowGrid").toBool()) {
		mWidthOfGrid = SettingsManager::value("GridWidth").toReal() / 100;
		painter->setPen(QPen(Qt::black, mWidthOfGrid));
		QGraphicsScene::drawBackground(painter, rect);
		int const cellSize = SettingsManager::value("2dGridCellSize").toInt();
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

void D2ModelScene::clearScene(bool removeRobot)
{
	mModel.worldModel().clear();
	mModel.robotModel().clear();
	if (removeRobot) {
		for (interpreterBase::robotModel::PortInfo const &port : mRobot->sensors().keys()) {
			deviceConfigurationChanged(mModel.robotModel().info().name()
					, port, interpreterBase::robotModel::DeviceInfo());
		}

		delete mRobot;
		clear();
		drawInitialRobot();
	}
}

void D2ModelScene::reshapeWall(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentWall) {
		QPointF const oldPos = mCurrentWall->end();
		mCurrentWall->setX2andY2(pos.x(), pos.y());
		if (SettingsManager::value("2dShowGrid").toBool()) {
			mCurrentWall->reshapeBeginWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			mCurrentWall->reshapeEndWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		} else {
			if (mCurrentWall->realShape().intersects(mRobot->realBoundingRect())) {
				mCurrentWall->setX2andY2(oldPos.x(), oldPos.y());
			}
			if (event->modifiers() & Qt::ShiftModifier) {
				mCurrentWall->reshapeRectWithShift();
			}
		}
	}
}

void D2ModelScene::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentLine) {
		mCurrentLine->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentLine->reshapeRectWithShift();
		}
	}
}

void D2ModelScene::reshapeStylus(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentStylus) {
		mCurrentStylus->addLine(pos.x(), pos.y());
	}
}

void D2ModelScene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentEllipse) {
		mCurrentEllipse->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentEllipse->reshapeRectWithShift();
		}
	}
}

void D2ModelScene::worldWallDragged(items::WallItem *wall, QPainterPath const &shape, QPointF const &oldPos)
{
	bool const isNeedStop = shape.intersects(mRobot->realBoundingRect());
	wall->onOverlappedWithRobot(isNeedStop);
	if (wall->isDragged() && ((mDrawingAction == none) ||
			(mDrawingAction == D2ModelScene::wall && mCurrentWall == wall)))
	{
		wall->setFlag(QGraphicsItem::ItemIsMovable, !isNeedStop);
		if (isNeedStop) {
			wall->setPos(oldPos);
		}
	}
}

void D2ModelScene::alignWalls()
{
	for (items::WallItem * const wall : mModel.worldModel().walls()) {
		if (items().contains(wall)) {
			wall->setBeginCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			wall->setEndCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		}
	}
}

RobotItem *D2ModelScene::robot()
{
	return mRobot;
}

void D2ModelScene::centerOnRobot()
{
	for (QGraphicsView * const view : views()) {
		QRectF const viewPortRect = view->mapToScene(view->viewport()->rect()).boundingRect();
		if (!viewPortRect.contains(mRobot->sceneBoundingRect().toRect())) {
			QRectF const requiredViewPort = viewPortRect.translated(mRobot->scenePos() - viewPortRect.center());
			setSceneRect(itemsBoundingRect().united(requiredViewPort));
			view->centerOn(mRobot);
		}
	}
}
