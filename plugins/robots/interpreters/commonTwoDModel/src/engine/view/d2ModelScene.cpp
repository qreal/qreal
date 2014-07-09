#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

#include <qrkernel/settingsManager.h>
#include <qrutils/graphicsUtils/gridDrawer.h>
#include "d2ModelScene.h"

#include "src/engine/items/wallItem.h"

using namespace twoDModel;
using namespace view;
using namespace qReal;
using namespace graphicsUtils;

D2ModelScene::D2ModelScene(AbstractView *view, QObject *parent)
	: AbstractScene(view, parent)
	, mDrawingAction(none)
	, mMouseClicksCount(0)
	, mCurrentWall(nullptr)
	, mCurrentLine(nullptr)
	, mCurrentStylus(nullptr)
	, mCurrentEllipse(nullptr)
	, mClearing(false)
{
	mFirstPenWidth = 6;
	mSizeEmptyRectX = 1000;
	mSizeEmptyRectY = 1000;
	setEmptyRect(-500, -500, mSizeEmptyRectX, mSizeEmptyRectY);
	setItemIndexMethod(NoIndex);
	setEmptyPenBrushItems();
}

void D2ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	mRobot->checkSelection();
	for (SensorItem *sensor : mRobot->sensors().values()) {
		if (sensor) {
			sensor->checkSelection();
		}
	}

	QPointF const position = mouseEvent->scenePos();
	processDragMode();

	switch (mDrawingAction) {
	case wall: {
		if (!mRobot->realBoundingRect().intersects(QRectF(position, position))) {
			mCurrentWall = new items::WallItem(position, position);
			mScene->removeMoveFlag(mouseEvent, mCurrentWall);
			mModel.worldModel().addWall(mCurrentWall);
			mMouseClicksCount++;
		}
		break;
	}
	case line: {
		mCurrentLine = new items::LineItem(position, position);
		mCurrentLine->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
				, mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentLine);
		mModel.worldModel().addColorField(mCurrentLine);
		mMouseClicksCount++;
		break;
	}
	case stylus: {
		mCurrentStylus = new items::StylusItem(position.x(), position.y());
		mCurrentStylus->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
				, mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentStylus);
		mModel.worldModel().addColorField(mCurrentStylus);
		mMouseClicksCount++;
		break;
	}
	case ellipse: {
		mCurrentEllipse = new items::EllipseItem(position, position);
		mCurrentEllipse->setPen(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentEllipse);
		mModel.worldModel().addColorField(mCurrentEllipse);
		mMouseClicksCount++;
		break;
	}
	case none: {
		mMouseClicksCount = 0;
		mScene->forPressResize(mouseEvent);
		break;
	}
	default:
		break;
	}

	update();
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
	processDragMode();
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
			mScene->forMoveResize(mouseEvent, mRobot->realBoundingRect());
		}
		break;
	}

	if (needUpdate) {
		mScene->update();
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

	processDragMode();

	switch (mDrawingAction){
	case wall: {
		reshapeWall(mouseEvent);
		mCurrentWall->setSelected(true);
		mCurrentWall = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case line: {
		reshapeLine(mouseEvent);
		mCurrentLine->setSelected(true);
		mCurrentLine = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case stylus: {
		reshapeStylus(mouseEvent);
		mCurrentStylus->setSelected(true);
		mCurrentStylus = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case ellipse: {
		reshapeEllipse(mouseEvent);
		mCurrentEllipse->setSelected(true);
		mCurrentEllipse = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	default:
		mScene->forReleaseResize(mouseEvent, mRobot->realBoundingRect());
		break;
	}

	mScene->setMoveFlag(mouseEvent);

	mScene->update();
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
	if (!scene()->items().contains(item)) {
		return;
	}

	/// @todo Handle all cases equally
	if (SensorItem * const sensor = dynamic_cast<SensorItem *>(item)) {
		PortInfo const port = mRobot->sensors().key(sensor);
		if (port.isValid()) {
			deviceConfigurationChanged(mModel.robotModel().info().name(), port, DeviceInfo());
		}

		return;
	}

	if (items::WallItem * const wall = dynamic_cast<items::WallItem *>(item)) {
		mScene->removeItem(wall);
		mModel.worldModel().removeWall(wall);
		return;
	}

	if (items::ColorFieldItem *colorField = dynamic_cast<items::ColorFieldItem *>(item)) {
		mScene->removeItem(colorField);
		mModel.worldModel().removeColorField(colorField);
		delete colorField;
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

void D2ModelScene::addWall(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->wallButton);
	setCursorTypeForDrawing(drawWall);
	mDrawingAction = wall;
}

void D2ModelScene::addLine(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->lineButton);
	setCursorTypeForDrawing(drawLine);
	mDrawingAction = line;
}

void D2ModelScene::addStylus(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->stylusButton);
	setCursorTypeForDrawing(drawStylus);
	mDrawingAction = stylus;
}

void D2ModelScene::addEllipse(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->ellipseButton);
	setCursorTypeForDrawing(drawEllipse);
	mDrawingAction = ellipse;
}

void D2ModelScene::setNoneButton()
{
	setHighlightOneButton(mUi->noneButton);
	setNoneStatus();
}

void D2ModelScene::setNoneStatus()
{
	mDrawingAction = none;
	mMouseClicksCount = 0;
	setCursorTypeForDrawing(mNoneCursorType);
	mUi->noneButton->setChecked(true);
}

void D2ModelScene::resetButtons()
{
	mCurrentWall = nullptr;
	mCurrentLine = nullptr;
	mCurrentStylus = nullptr;
	mMouseClicksCount = 0;
	mDrawingAction = none;
}

void D2ModelScene::drawColorFields()
{
	if (mDrawingAction == line
			|| mDrawingAction == stylus
			|| mDrawingAction == ellipse
			|| mDrawingAction == noneWordLoad) {
		for (items::ColorFieldItem *colorField : mModel.worldModel().colorFields()) {
			if (!mScene->items().contains(colorField)) {
				mScene->addItem(colorField);
			}
		}
	}
}

void D2ModelWidget::drawWalls()
{
	if (mDrawingAction == wall || mDrawingAction == noneWordLoad) {
		for (items::WallItem *wall : mModel.worldModel().walls()) {
			if (!mScene->items().contains(wall)) {
				mScene->addItem(wall);
				connect(wall, &items::WallItem::wallDragged, this, &D2ModelWidget::worldWallDragged);
			}
		}
	}
}

void D2ModelScene::clearScene(bool removeRobot)
{
	mClearing = true;
	mModel.worldModel().clearScene();
	mModel.robotModel().clear();
	if (removeRobot) {
		for (PortInfo const &port : mRobot->sensors().keys()) {
			deviceConfigurationChanged(mModel.robotModel().info().name(), port, DeviceInfo());
		}

		delete mRobot;
		mScene->clear();
		drawInitialRobot();
	} else {
		for (QGraphicsItem * const item : mScene->items()) {
			if (item != mRobot && !mRobot->isAncestorOf(item)) {
				mScene->removeItem(item);
				delete item;
			}
		}
	}

	mClearing = false;
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
			(mDrawingAction == D2ModelWidget::wall && mCurrentWall == wall)))
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
		if (mScene->items().contains(wall)) {
			wall->setBeginCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			wall->setEndCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		}
	}
}
