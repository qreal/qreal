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

#include <QtWidgets/QMenu>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include "abstractScene.h"
#include "abstractItem.h"

using namespace graphicsUtils;

AbstractScene::AbstractScene(AbstractView *view, QObject *parent)
		: QGraphicsScene(parent)
		, mView(view)
		, mGraphicsItem(nullptr)
		, mX1(0)
		, mX2(0)
		, mY1(0)
		, mY2(0)
		, mSizeEmptyRectX(0)
		, mSizeEmptyRectY(0)
		, mPenWidthItems(0)
		, mFirstPenWidth(0)
		, mEmptyRect(nullptr)
{
}

graphicsUtils::AbstractView* AbstractScene::mainView()
{
	return mView;
}

void AbstractScene::setEmptyRect(int x, int y, int w, int h)
{
	mEmptyRect = addRect(x, y, w, h, QPen(Qt::white));
}

QRect AbstractScene::realItemsBoundingRect() const
{
	const QRectF rect = itemsBoundingRect();
	int maxX = static_cast<int>(rect.left());
	int maxY = static_cast<int>(rect.top());
	int minY = static_cast<int>(rect.bottom());
	int minX = static_cast<int>(rect.right());
	QList<QGraphicsItem *> list = items();

	foreach (QGraphicsItem *graphicsItem, list) {
		AbstractItem* item = dynamic_cast<AbstractItem*>(graphicsItem);
		if (item) {
			const QRectF itemRect = item->realBoundingRect();
			maxX = qMax(static_cast<int>(itemRect.right()), maxX);
			maxY = qMax(static_cast<int>(itemRect.bottom()), maxY);
			minX = qMin(static_cast<int>(itemRect.left()), minX);
			minY = qMin(static_cast<int>(itemRect.top()), minY);
		}
	}
	return QRect(minX, minY, maxX - minX, maxY - minY);
}

void AbstractScene::setX1andY1(QGraphicsSceneMouseEvent *event)
{
	mX1 = event->scenePos().x();
	mY1 = event->scenePos().y();
}

void AbstractScene::setX2andY2(QGraphicsSceneMouseEvent *event)
{
	mX2 = event->scenePos().x();
	mY2 = event->scenePos().y();
}

void AbstractScene::reshapeItem(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	if (mGraphicsItem && mGraphicsItem->editable()) {
		if (mGraphicsItem->dragState() != graphicsUtils::AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}
		mGraphicsItem->resizeItem(event);
	}
}

void AbstractScene::reshapeItem(QGraphicsSceneMouseEvent *event,graphicsUtils::AbstractItem *item)
{
	setX2andY2(event);
	if (item) {
		if (item->dragState() != graphicsUtils::AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}
		item->resizeItem(event);
	}
}

void AbstractScene::setMoveFlag(QGraphicsSceneMouseEvent *event)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list){
		AbstractItem *item = dynamic_cast<graphicsUtils::AbstractItem *>(graphicsItem);
		if (item && item->editable()) {
			graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, true);
		}
	}
}

void AbstractScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mousePressEvent(mouseEvent);
	if (mouseEvent->button() == Qt::LeftButton) {
		emit leftButtonPressed();
	}
}

void AbstractScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	if (mouseEvent->button() == Qt::LeftButton) {
		emit leftButtonReleased();
	}
}

void AbstractScene::removeMoveFlag(QGraphicsSceneMouseEvent *event, QGraphicsItem *item)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list) {
		AbstractItem *grItem = dynamic_cast<graphicsUtils::AbstractItem *>(graphicsItem);
		if (grItem) {
			grItem->setFlag(QGraphicsItem::ItemIsMovable, false);
		}
	}

	if (item && item != mEmptyRect) {
		item->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
}

void AbstractScene::setDragMode(int itemsType)
{
	if (itemsType) {
		mView->setDragMode(QGraphicsView::NoDrag);
	} else {
		mView->setDragMode(QGraphicsView::RubberBandDrag);
	}
}

void AbstractScene::setDragMode(QGraphicsView::DragMode mode)
{
	mView->setDragMode(mode);
}

void AbstractScene::forPressResize(QGraphicsSceneMouseEvent *event)
{
	setX1andY1(event);
	mGraphicsItem = dynamic_cast<AbstractItem *>(itemAt(event->scenePos(), QTransform()));
	if (mGraphicsItem && mGraphicsItem->editable()) {
		mGraphicsItem->changeDragState(mX1, mY1);
		if (mGraphicsItem->dragState() != AbstractItem::None) {
			mView->setDragMode(QGraphicsView::NoDrag);
		}
	}
	update();
}

void AbstractScene::forMoveResize(QGraphicsSceneMouseEvent *event)
{
	reshapeItem(event);
	update();
}

void AbstractScene::forReleaseResize(QGraphicsSceneMouseEvent * event )
{
	reshapeItem(event);
	mGraphicsItem = nullptr;
	update();
}

bool AbstractScene::compareItems(AbstractItem* first, AbstractItem* second)
{
	return first->zValue() < second->zValue();
}

void AbstractScene::setEmptyPenBrushItems()
{
	mPenStyleItems = "Solid";
	mPenWidthItems = mFirstPenWidth;
	mPenColorItems = "black";
	mBrushStyleItems = "None";
	mBrushColorItems = "white";
}

void AbstractScene::setPenBrushItems(const QPen &pen, const QBrush &brush)
{
	mPenStyleItems = convertPenToString(pen);
	mPenWidthItems = pen.width();
	mPenColorItems = pen.color().name();
	mBrushStyleItems = convertBrushToString(brush);
	mBrushColorItems = brush.color().name();
}

QString AbstractScene::convertPenToString(const QPen &pen)
{
	QString penStyle;
	switch (pen.style()) {
	case Qt::SolidLine:
		penStyle = "Solid";
		break;
	case Qt::DotLine:
		penStyle = "Dot";
		break;
	case Qt::DashLine:
		penStyle = "Dash";
		break;
	case Qt::DashDotLine:
		penStyle = "DashDot";
		break;
	case Qt::DashDotDotLine:
		penStyle = "DashDotDot";
		break;
	case Qt::NoPen:
		penStyle = "None";
		break;
	default:
		break;
	}
	return penStyle;
}

QString AbstractScene::convertBrushToString(const QBrush &brush)
{
	switch (brush.style()) {
	case Qt::NoBrush:
		return "None";
	case Qt::SolidPattern:
		return "Solid";
	default:
		return QString();
	}
}

QString AbstractScene::penStyleItems()
{
	return mPenStyleItems;
}

int AbstractScene::penWidthItems()
{
	return mPenWidthItems;
}

QString AbstractScene::penColorItems()
{
	return mPenColorItems;
}

QString AbstractScene::brushStyleItems()
{
	return mBrushStyleItems;
}

QString AbstractScene::brushColorItems()
{
	return mBrushColorItems;
}

QList<AbstractItem *> AbstractScene::abstractItems(const QPointF &scenePos) const
{
	QList<AbstractItem *> result;
	for (QGraphicsItem * const item : items(scenePos)) {
		if (AbstractItem * const abstractItem = dynamic_cast<AbstractItem *>(item)) {
			result << abstractItem;
		}
	}

	return result;
}

void AbstractScene::setPenStyleItems(const QString &text)
{
	mPenStyleItems = text;
}

void AbstractScene::setPenWidthItems(int width)
{
	mPenWidthItems = width;
}

void AbstractScene::setPenColorItems(const QString &text)
{
	mPenColorItems = text;
}

void AbstractScene::setBrushStyleItems(const QString &text)
{
	mBrushStyleItems = text;
}

void AbstractScene::setBrushColorItems(const QString &text)
{
	mBrushColorItems = text;
}

void AbstractScene::addAction(QAction * const action)
{
	mActions << action;
	mView->addAction(action);
}

void AbstractScene::addActions(const QList<QAction *> &actions)
{
	mActions << actions;
	mView->addActions(actions);
}

void AbstractScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	if (abstractItems(event->scenePos()).isEmpty()) {
		QMenu menu;
		menu.addActions(mActions);
		if (!menu.isEmpty()) {
			menu.exec(event->screenPos());
		}
	} else {
		QGraphicsScene::contextMenuEvent(event);
	}
}
