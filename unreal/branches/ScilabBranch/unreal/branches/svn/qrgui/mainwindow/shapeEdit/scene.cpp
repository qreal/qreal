#include "scene.h"

#include <QtCore/QPointF>

Scene::Scene(QGraphicsView *view, QObject * parent)
	:  QGraphicsScene(parent), mItemType(none), mWaitMove(false), mCount(0), mGraphicsItem(NULL)
{
	mView = view;
	setItemIndexMethod(NoIndex);
	mEmptyRect = addRect(0, 0, sizeEmrtyRectX, sizeEmrtyRectY, QPen(Qt::white));
	setEmptyPenBrushItems();
	connect(this, SIGNAL(selectionChanged ()), this, SLOT(changePalette()));
}

void Scene::setEmptyPenBrushItems()
{
	mPenStyleItems = "Solid";
	mPenWidthItems = 0;
	mPenColorItems = "black";
	mBrushStyleItems = "None";
	mBrushColorItems = "white";
}

QPointF Scene::centerEmpty()
{
	return QPointF(sizeEmrtyRectX / 2, sizeEmrtyRectY / 2);
}

void Scene::setX1andY1(QGraphicsSceneMouseEvent *event)
{
	mX1 = event->scenePos().x();
	mY1 = event->scenePos().y();
}

void Scene::setX2andY2(QGraphicsSceneMouseEvent *event)
{
	mX2 = event->scenePos().x();
	mY2 = event->scenePos().y();
}

QPointF Scene::setCXandCY(QGraphicsSceneMouseEvent *event)
{
	qreal x = event->scenePos().x();
	qreal y = event->scenePos().y();
	return QPointF(x, y);
}

void Scene::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mLine->setX2andY2(mX2, mY2);
}

void Scene::reshapeLinePort(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mLinePort->setX2andY2(mX2, mY2);
}

void Scene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mEllipse->setX2andY2(mX2, mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mEllipse->reshapeRectWithShift();
}

void Scene::reshapeRectangle(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mRectangle->setX2andY2(mX2, mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mRectangle->reshapeRectWithShift();
}

void Scene::reshapeStylus(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mStylus->addLine(mX2, mY2);
}

void Scene::reshapeCurveFirst(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mCurve->setX2andY2(mX2, mY2);
}

void Scene::reshapeCurveSecond(QGraphicsSceneMouseEvent *event)
{
	mC1 = setCXandCY(event);
	mCurve->setCXandCY(mC1.x(), mC1.y());
}

void Scene::reshapeItem(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	if (mGraphicsItem != NULL) {
		if (mGraphicsItem->getDragState() != Item::None)
			mView->setDragMode(QGraphicsView::NoDrag);
		mGraphicsItem->resizeItem(event);
	}
}

void Scene::reshapeItem(QGraphicsSceneMouseEvent *event, Item *item)
{
	setX2andY2(event);
	if (item != NULL) {
		if (item->getDragState() != Item::None)
			mView->setDragMode(QGraphicsView::NoDrag);
		item->resizeItem(event);
	}
}

void Scene::removeMoveFlag(QGraphicsSceneMouseEvent *event, QGraphicsItem* item)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list) {
		Item *grItem = dynamic_cast<Item *>(graphicsItem);
		if (grItem != NULL)
			grItem->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
	if (item!= NULL && item != mEmptyRect)
		item->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Scene::setMoveFlag(QGraphicsSceneMouseEvent *event)
{
	QList<QGraphicsItem *> list = items(event->scenePos());
	foreach (QGraphicsItem *graphicsItem, list){
		Item *item = dynamic_cast<Item *>(graphicsItem);
		if (item != NULL)
			graphicsItem->setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

void Scene::setZValueItems(int index)
{
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		Text* item = dynamic_cast<Text*>(graphicsItem);
		if (item != NULL)
			item->setZValueAll(index);
		else
			graphicsItem->setZValue(index);
	}
}

QPair<bool, Item *> Scene::checkOnResize(qreal x, qreal y)
{
	QList<Item *> list = selectedSceneItems();
	foreach (Item *item, list) {
		item->changeDragState(x, y);
		if (item->getDragState() != Item::None)  {
			mView->setDragMode(QGraphicsView::NoDrag);
			return QPair<bool, Item *>(true, item);
		}
	}
	return QPair<bool, Item *>(false, NULL);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if (mItemType != none)
		mView->setDragMode(QGraphicsView::NoDrag);
	switch (mItemType) {
	case curve:
		if (mCount == 1) {
			setX1andY1(event);
			mNeedResize = checkOnResize(mX1, mY1);
			if (mNeedResize.first) {
				mWaitMove = true;
				mCount = 1;
				break;
			}
		}
		else if (mCount == 2)
			reshapeCurveFirst(event);
		else if (mCount == 3)
			reshapeCurveSecond(event);
		removeMoveFlag(event, NULL);
		mCount++;
		break;
	case stylus :
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mStylus = new Stylus(mX1, mY1, NULL);
			mStylus->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
			addItem(mStylus);
			removeMoveFlag(event, mStylus);
		}
		mWaitMove = true;
		break;
	case line :
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mLine = new Line(mX1, mY1, mX1, mY1, NULL);
			mLine->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
			addItem(mLine);
			removeMoveFlag(event, mLine);
		}
		mWaitMove = true;
		break;
	case ellipse :
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mEllipse = new Ellipse(mX1, mY1, mX1, mY1, NULL);
			mEllipse->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
			addItem(mEllipse);
			removeMoveFlag(event, mEllipse);
		}
		mWaitMove = true;
		break;
	case rectangle :
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mRectangle = new Rectangle(mX1, mY1, mX1, mY1, NULL);
			mRectangle->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
			addItem(mRectangle);
			removeMoveFlag(event, mRectangle);
		}
		mWaitMove = true;
		break;
	case text:
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mText = new Text(mX1, mY1, "text", false);
			addItem(mText);
		} else
			mWaitMove = true;//for resize
		break;
	case dynamicText :
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mText = new Text(mX1, mY1, "name", true);
			addItem(mText);
		} else
			mWaitMove = true;//for resize
		break;
	case pointPort :
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mPointPort = new PointPort(mX1, mY1, NULL);
			addItem(mPointPort);
		} else
			mWaitMove = true;//for resize
		break;
	case linePort :
		setX1andY1(event);
		mNeedResize = checkOnResize(mX1, mY1);
		if (!mNeedResize.first) {
			mLinePort = new LinePort(mX1, mY1, mX1, mY1, NULL);
			addItem(mLinePort);
			removeMoveFlag(event, mLinePort);
		}
		mWaitMove = true;
		break;
	default:  // if we wait some resize
		setX1andY1(event);
		mGraphicsItem = dynamic_cast<Item *>(itemAt(event->scenePos()));
		if (mGraphicsItem != NULL) {
			mGraphicsItem->changeDragState(mX1, mY1);
			if (mGraphicsItem->getDragState() != Item::None)
				mView->setDragMode(QGraphicsView::NoDrag);
		}
		break;
	}
	setZValueItems(items().size() + 1);
}

void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (mItemType != none)
		mView->setDragMode(QGraphicsView::NoDrag);
	switch (mItemType) {
	case stylus :
		if (mWaitMove) {
			if (!mNeedResize.first)
				reshapeStylus(event);
			else
				reshapeItem(event, mNeedResize.second);
		}
		break;
	case line :
		if (mWaitMove) {
			if (!mNeedResize.first)
				reshapeLine(event);
			else
				reshapeItem(event, mNeedResize.second);
		}
		break;
	case ellipse :
		if (mWaitMove) {
			if (!mNeedResize.first)
				reshapeEllipse(event);
			else
				reshapeItem(event, mNeedResize.second);
		}
		break;
	case rectangle :
		if (mWaitMove) {
			if (!mNeedResize.first)
				reshapeRectangle(event);
			else
				reshapeItem(event, mNeedResize.second);
		}
		break;
	case curve:
		if (mCount == 2) {
			if (mCurve != NULL)
				reshapeCurveFirst(event);
		} else if (mCount == 3)
			reshapeCurveSecond(event);
	case text:
	case dynamicText :
	case pointPort :
		if (mWaitMove && mNeedResize.first) {
			reshapeItem(event, mNeedResize.second);
		}
		break;
	case linePort :
		if (mWaitMove) {
			if (!mNeedResize.first)
				reshapeLinePort(event);
			else
				reshapeItem(event, mNeedResize.second);
		}
		break;
	default:  // if we wait some resize
		reshapeItem(event);
		break;
	}
	setZValueItems(items().size() + 1);
	update();
}

void Scene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	if (mItemType != none)
		mView->setDragMode(QGraphicsView::NoDrag);
	switch (mItemType) {
	case curve:
		if (mCount == 2) {
			setX2andY2(event);
			mCurve = new Curve(QPointF(mX1, mY1), QPointF(mX2, mY2), QPointF(mX1, mY1));
			mCurve->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
			addItem(mCurve);
		} else if (mCount == 3)
			reshapeCurveSecond(event);
		else if (mCount == 4) {
			mCount = 1;
			mCurve = NULL;
		}
		break;
	case stylus :
		if (!mNeedResize.first)
			reshapeStylus(event);
		else
			reshapeItem(event, mNeedResize.second);
		break;
	case line :
		if (!mNeedResize.first)
			reshapeLine(event);
		else
			reshapeItem(event, mNeedResize.second);
		break;
	case ellipse :
		if (!mNeedResize.first)
			reshapeEllipse(event);
		else
			reshapeItem(event, mNeedResize.second);
		break;
	case rectangle :
		if (!mNeedResize.first)
			reshapeRectangle(event);
		else
			reshapeItem(event, mNeedResize.second);
		break;
	case text:
	case dynamicText :
	case pointPort :
		if (mNeedResize.first)
			reshapeItem(event, mNeedResize.second);
		break;
	case linePort :
		if (!mNeedResize.first)
			reshapeLinePort(event);
		else
			reshapeItem(event, mNeedResize.second);
		break;
	default:  // if we wait some resize
		reshapeItem(event);
		break;
	}
	setZValueItems(0);
	mWaitMove = false;
	mGraphicsItem = NULL;
	setMoveFlag(event);
	mView->setDragMode(QGraphicsView::RubberBandDrag);
}

void Scene::drawLine(bool checked)
{
	if (checked)
		mItemType = line;
}

void Scene::drawEllipse(bool checked)
{
	if (checked)
		mItemType = ellipse;
}

void Scene::drawCurve(bool checked)
{
	if (checked) {
		mItemType = curve;
		mCount = 1;
	}
}

void Scene::drawRectangle(bool checked)
{
	if (checked)
		mItemType = rectangle;
}

void Scene::addText(bool checked)
{
	if (checked)
		mItemType = text;
}

void Scene::addDynamicText(bool checked)
{
	if (checked)
		mItemType = dynamicText;
}

void Scene::addPointPort(bool checked)
{
	if (checked)
		mItemType = pointPort;
}

void Scene::addLinePort(bool checked)
{
	if (checked)
		mItemType = linePort;
}

void Scene::addStylus(bool checked)
{
	if (checked)
		mItemType = stylus;
}

void Scene::addNone(bool checked)
{
	if (checked) {
		mItemType = none;
		mCount = 0;
	}
}

void Scene::deleteItem()
{
	QList<QGraphicsItem *> list = selectedItems();
	foreach (QGraphicsItem *graphicsItem, list) {
		removeItem(graphicsItem);
		delete graphicsItem;
	}
}

void Scene::clearScene()
{
	clear();
	mEmptyRect = addRect(0, 0, sizeEmrtyRectX, sizeEmrtyRectY, QPen(Qt::white));
}

QList<Item *> Scene::selectedSceneItems()
{
	QList<Item *> resList;
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		Item* item = dynamic_cast<Item*>(graphicsItem);
		if (item != NULL)
			resList.push_back(item);
	}
	return resList;
}

void Scene::changePenStyle(const QString &text)
{
	mPenStyleItems = text;
	foreach (Item *item, selectedSceneItems())
		item->setPenStyle(text);
	update();
}

void Scene::changePenWidth(int width)
{
	mPenWidthItems = width;
	foreach (Item *item, selectedSceneItems())
		item->setPenWidth(width);
	update();
}

void Scene::changePenColor(const QString &text)
{
	mPenColorItems = text;
	foreach (Item *item, selectedSceneItems())
		item->setPenColor(text);
	update();
}

void Scene::changeBrushStyle(const QString &text)
{
	mBrushStyleItems = text;
	foreach (Item *item, selectedSceneItems())
		item->setBrushStyle(text);
	update();
}

void Scene::changeBrushColor(const QString &text)
{
	mBrushColorItems = text;
	foreach (Item *item, selectedSceneItems())
		item->setBrushColor(text);
	update();
}

QString Scene::convertPenToString(QPen const &pen)
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
		penStyle =  "DashDot";
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

QString Scene::convertBrushToString(QBrush const &brush)
{
	QString brushStyle;
	if (brush.style() == Qt::NoBrush)
		brushStyle = "None";
	if (brush.style() == Qt::SolidPattern)
		brushStyle = "Solid";
	return brushStyle;
}

void Scene::setPenBrushItems(QPen const &pen, QBrush const &brush)
{
	mPenStyleItems = convertPenToString(pen);
	mPenWidthItems = pen.width();
	mPenColorItems = pen.color().name();
	mBrushStyleItems = convertBrushToString(brush);
	mBrushColorItems = brush.color().name();
}

void Scene::changePalette()
{
	if(mItemType == none) {
		mListSelectedItems = selectedItems();
		if (mListSelectedItems.isEmpty()) {
			emit noSelectedItems();
			setEmptyPenBrushItems();
		}
		else {
			Item* item = dynamic_cast<Item*>(mListSelectedItems.back());
			if (item != NULL) {
				QPen penItem = item->pen();
				QBrush brushItem = item->brush();
				emit existSelectedItems(penItem, brushItem);
				setPenBrushItems(penItem, brushItem);
			}
		}
	}
}
