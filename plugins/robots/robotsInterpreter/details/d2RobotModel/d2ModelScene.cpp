#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>

#include "d2ModelScene.h"
#include "../../../../../qrutils/graphicsUtils/griddrawer.h"
#include "../../../../../qrkernel/settingsManager.h"

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
    gd = new GridDrawer;
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

void D2ModelScene::forPressResize(QGraphicsSceneMouseEvent *event)
{
	setX1andY1(event);
	mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(event->scenePos()));
	if (mGraphicsItem) {
		mGraphicsItem->changeDragState(mX1, mY1);
		if (mGraphicsItem->getDragState() != AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}
	}
	update();
}

void D2ModelScene::forMoveResize(QGraphicsSceneMouseEvent *event, QRectF const &rect)
{
	reshapeItem(event, rect);
	update();
}

void D2ModelScene::forReleaseResize(QGraphicsSceneMouseEvent * event, QRectF const &rect)
{
	reshapeItem(event, rect);
	mGraphicsItem = NULL;
	update();
}

void D2ModelScene::reshapeItem(QGraphicsSceneMouseEvent *event, QRectF const &rect)
{
	setX2andY2(event);
	if (mGraphicsItem) {
		QPointF oldBegin = mGraphicsItem->getX1andY1();
		QPointF oldEnd = mGraphicsItem->getX2andY2();
		if (mGraphicsItem->getDragState() != graphicsUtils::AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}
		mGraphicsItem->resizeItem(event);

		if (mGraphicsItem->realShape().intersects(rect)) {
			mGraphicsItem->reverseOldResizingItem(oldBegin, oldEnd);
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

void D2ModelScene::drawBackground ( QPainter * painter, const QRectF & rect )
{
    if (SettingsManager::value("2dShowGrid").toBool()){
        QGraphicsScene::drawBackground(painter, rect);
        int cellSize = SettingsManager::value("2dGridCellSize").toInt();
        gd -> drawGrid (painter, rect, cellSize);
    }
}

void D2ModelScene::updateGrid()
{
    this->update();
}
