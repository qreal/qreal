#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>

#include "d2ModelScene.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

D2ModelScene::D2ModelScene(AbstractView *view, QObject *parent)
		: AbstractScene(view, parent)
{
	mFirstPenWidth = 6;
	mSizeEmptyRectX = 1000;
	mSizeEmptyRectY = 1000;
	setEmptyRect(-500, -500, mSizeEmptyRectX, mSizeEmptyRectY);
	setItemIndexMethod(NoIndex);
	setEmptyPenBrushItems();
}

D2ModelScene::~D2ModelScene()
{
}

void D2ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mousePressEvent(mouseEvent);
	emit mousePressed(mouseEvent);
}

void D2ModelScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	emit mouseReleased(mouseEvent);
}

void D2ModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	emit mouseMoved(mouseEvent);
}

void D2ModelScene::forPressResize(QGraphicsSceneMouseEvent *event, QRectF const &rect) //asd
{
	QPointF oldPos = QPointF(mX1, mY1);
	setX1andY1(event);
	mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(event->scenePos()));
	if (mGraphicsItem != NULL) {
		if (mGraphicsItem->boundingRect().intersects(rect)) {
			mView->setDragMode(QGraphicsView::NoDrag);
			mX1 = oldPos.x();
			mY1 = oldPos.y();
			mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(oldPos));
		} else {
			mGraphicsItem->changeDragState(mX1, mY1);
			mView->setDragMode(QGraphicsView::RubberBandDrag);
			if (mGraphicsItem->getDragState() != AbstractItem::None) {
				mView->setDragMode(QGraphicsView::NoDrag);
			}
		}
	}
	update();
}

void D2ModelScene::forMoveResize(QGraphicsSceneMouseEvent *event, QRectF const &rect) //asd
{
	reshapeItem(event, rect);
	update();
}

void D2ModelScene::forReleaseResize(QGraphicsSceneMouseEvent * event, QRectF const &rect) //asd
{
	reshapeItem(event, rect);
	mGraphicsItem = NULL;
	update();
}

void D2ModelScene::reshapeItem(QGraphicsSceneMouseEvent *event, QRectF const &rect)
{
	QPointF oldPos = QPointF(mX1, mY1);
	setX2andY2(event);
	if (mGraphicsItem != NULL) {
		if (mGraphicsItem->boundingRect().intersects(rect)) {
			mView->setDragMode(QGraphicsView::NoDrag);
			mX1 = oldPos.x();
			mY1 = oldPos.y();
			mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(oldPos));
		} else {
			mView->setDragMode(QGraphicsView::RubberBandDrag);
			if (mGraphicsItem->getDragState() != graphicsUtils::AbstractItem::None) {
				mView->setDragMode(QGraphicsView::NoDrag);
			}
			mGraphicsItem->resizeItem(event);
		}
	}
}

void D2ModelScene::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && (selectedItems().size() > 0)) {
		foreach(QGraphicsItem* item, selectedItems()) {
			emit itemDeleted(item);
		}
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}
