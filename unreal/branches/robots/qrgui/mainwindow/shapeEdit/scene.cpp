#include "scene.h"

#include <QtCore/QPointF>
#include <QtGui/QKeyEvent>
#include <QtCore/QFile>
#include <QtCore/QSettings>
#include <QtCore/QDir>

Scene::Scene(View *view, QObject * parent)
	:  QGraphicsScene(parent), mItemType(none), mWaitMove(false), mCount(0), mGraphicsItem(NULL), mSelectedTextPicture(NULL)
{
	mView = view;
	setItemIndexMethod(NoIndex);
	mEmptyRect = addRect(0, 0, sizeEmptyRectX, sizeEmptyRectY, QPen(Qt::white));
	setEmptyPenBrushItems();
	mCopyPaste = nonePaste;
	connect(this, SIGNAL(selectionChanged()), this, SLOT(changePalette()));
	connect(this, SIGNAL(selectionChanged()), this, SLOT(changeFontPalette()));
	mZValue = 0;
}

QRect Scene::realItemsBoundingRect() const
{
	QRectF rect = itemsBoundingRect();
	int maxX = static_cast<int>(rect.left());
	int maxY = static_cast<int>(rect.top());
	int minY = static_cast<int>(rect.bottom());
	int minX = static_cast<int>(rect.right());
	QList<QGraphicsItem *> list = items();
	foreach (QGraphicsItem *graphicsItem, list) {

		Item* item = dynamic_cast<Item*>(graphicsItem);
		if (item != NULL) {
			QRectF itemRect = item->realBoundingRect();
			maxX = qMax(static_cast<int>(itemRect.right()), maxX);
			maxY = qMax(static_cast<int>(itemRect.bottom()), maxY);
			minX = qMin(static_cast<int>(itemRect.left()), minX);
			minY = qMin(static_cast<int>(itemRect.top()), minY);
		}
	}
	return QRect(minX, minY, maxX - minX, maxY - minY);
}

QRectF Scene::selectedItemsBoundingRect() const
{
	QRectF resBoundRect;
	QList<Item *> list = mListSelectedItemsForPaste;
	foreach (Item *graphicsItem, list)
		resBoundRect |= graphicsItem->sceneBoundingRect();
	return resBoundRect;
}

void Scene::setEmptyPenBrushItems()
{
	mPenStyleItems = "Solid";
	mPenWidthItems = 0;
	mPenColorItems = "black";
	mBrushStyleItems = "None";
	mBrushColorItems = "white";
}

QPoint Scene::centerEmpty()
{
	return QPoint(sizeEmptyRectX / 2, sizeEmptyRectY / 2);
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
	if (event->modifiers() & Qt::ShiftModifier)
		mLine->reshapeRectWithShift();
}

void Scene::reshapeLinePort(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mLinePort->setX2andY2(mX2, mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mLinePort->reshapeRectWithShift();
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

void Scene::setZValue(Item* item)
{
	item->setItemZValue(mZValue);
	mZValue++;
}

void Scene::setZValueSelectedItems()
{
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		Item* item = dynamic_cast<Item*>(graphicsItem);
		item->setZValue(mZValue);
		mZValue++;
	}
}

void Scene::setNullZValueItems()
{
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		Item* item = dynamic_cast<Item*>(graphicsItem);
		item->setZValue(item->itemZValue());
	}
	mListSelectedItems.clear();
}

QPair<bool, Item *> Scene::checkOnResize(qreal x, qreal y)
{
	QList<Item *> list = selectedSceneItems();
	foreach (Item *item, list) {
		item->changeDragState(x, y);
		item->changeScalingPointState(x, y);
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
	else
		mView->setDragMode(QGraphicsView::RubberBandDrag);
	switch (mItemType) {
	case curve:
		if (mCount == 1)
			setX1andY1(event);
		else if (mCount == 2)
			reshapeCurveFirst(event);
		else if (mCount == 3)
			reshapeCurveSecond(event);
		removeMoveFlag(event, NULL);
		mCount++;
		break;
	case stylus :
		setX1andY1(event);
		mStylus = new Stylus(mX1, mY1, NULL);
		mStylus->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
		addItem(mStylus);
		setZValue(mStylus);
		removeMoveFlag(event, mStylus);
		mWaitMove = true;
		break;
	case line :
		setX1andY1(event);
		mLine = new Line(mX1, mY1, mX1, mY1, NULL);
		mLine->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
		addItem(mLine);
		setZValue(mLine);
		removeMoveFlag(event, mLine);
		mWaitMove = true;
		break;
	case ellipse :
		setX1andY1(event);
		mEllipse = new Ellipse(mX1, mY1, mX1, mY1, NULL);
		mEllipse->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
		addItem(mEllipse);
		setZValue(mEllipse);
		removeMoveFlag(event, mEllipse);
		mWaitMove = true;
		break;
	case rectangle :
		setX1andY1(event);
		mRectangle = new Rectangle(mX1, mY1, mX1, mY1, NULL);
		mRectangle->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
		addItem(mRectangle);
		setZValue(mRectangle);
		removeMoveFlag(event, mRectangle);
		mWaitMove = true;
		break;
	case text:
		setX1andY1(event);
		mText = new Text(mX1, mY1, "text", false);
		addItem(mText);
		setZValue(mText);
		break;
	case dynamicText :
		setX1andY1(event);
		mText = new Text(mX1, mY1, "name", true);
		addItem(mText);
		setZValue(mText);
		break;
	case textPicture:
		setX1andY1(event);
		mTextPicture = new TextPicture(mX1, mY1, "text");
		addItem(mTextPicture);
		setZValue(mTextPicture);
		break;
	case pointPort :
		setX1andY1(event);
		mPointPort = new PointPort(mX1, mY1, NULL);
		addItem(mPointPort);
		setZValue(mPointPort);
		break;
	case linePort :
		setX1andY1(event);
		mLinePort = new LinePort(mX1, mY1, mX1, mY1, NULL);
		addItem(mLinePort);
		setZValue(mLinePort);
		removeMoveFlag(event, mLinePort);
		mWaitMove = true;
		break;
	case image :
		setX1andY1(event);
		mImage = new Image(mFileName, mX1, mY1, NULL);
		addItem(mImage);
		setZValue(mImage);
		break;
	default:  // if we wait some resize
		setX1andY1(event);
		mGraphicsItem = dynamic_cast<Item *>(itemAt(event->scenePos()));
		if (mGraphicsItem != NULL) {
			mGraphicsItem->changeDragState(mX1, mY1);
			mGraphicsItem->changeScalingPointState(mX1, mY1);
			if (mGraphicsItem->getDragState() != Item::None)
				mView->setDragMode(QGraphicsView::NoDrag);
			if (mGraphicsItem->getScalingPointState() != Item::noneScale) {
				mGraphicsItem->setScalingPointColor();
				update();
			}
		}
		setZValueSelectedItems();
		break;
	}
}

void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (mItemType != none)
		mView->setDragMode(QGraphicsView::NoDrag);
	switch (mItemType) {
	case stylus :
		if (mWaitMove)
			reshapeStylus(event);
		break;
	case line :
		if (mWaitMove)
			reshapeLine(event);
		break;
	case ellipse :
		if (mWaitMove)
			reshapeEllipse(event);
		break;
	case rectangle :
		if (mWaitMove)
			reshapeRectangle(event);
		break;
	case curve:
		if (mCount == 2) {
			if (mCurve != NULL)
				reshapeCurveFirst(event);
		} else if (mCount == 3)
			reshapeCurveSecond(event);
		break;
	case linePort :
		if (mWaitMove)
			reshapeLinePort(event);
		break;
	default:  // if we wait some resize
		reshapeItem(event);
		break;
	}
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
			setZValue(mCurve);
		} else if (mCount == 3)
			reshapeCurveSecond(event);
		else if (mCount == 4) {
			mCount = 1;
			mCurve = NULL;
		}
		break;
	case stylus :
		reshapeStylus(event);
		break;
	case line :
		reshapeLine(event);
		break;
	case ellipse :
		reshapeEllipse(event);
		break;
	case rectangle :
		reshapeRectangle(event);
		break;
	case linePort :
		reshapeLinePort(event);
		break;
	default:  // if we wait some resize
		reshapeItem(event);
		break;
	}
	mWaitMove = false;
	mGraphicsItem = NULL;
	setMoveFlag(event);
	setNullZValueItems();
	mView->setDragMode(QGraphicsView::RubberBandDrag);
}

void Scene::initListSelectedItemsForPaste()
{
	mListSelectedItemsForPaste.clear();
	mListSelectedItemsForPaste = selectedSceneItems();
}

void Scene::keyPressEvent(QKeyEvent *keyEvent)
{
	QGraphicsScene::keyPressEvent(keyEvent);
	if (keyEvent->matches(QKeySequence::Cut)) {
		initListSelectedItemsForPaste();
		mCopyPaste = cut;
	} else if (keyEvent->matches(QKeySequence::Copy)) {
		initListSelectedItemsForPaste();
		mCopyPaste = copy;
	}
	else if (keyEvent->matches(QKeySequence::Paste)) {
		QPointF posCursor(mView->mapFromGlobal(QCursor::pos()));
		posCursor = mView->mapToScene(posCursor.toPoint());
		QPointF topLeftSelection(selectedItemsBoundingRect().topLeft());
		switch (mCopyPaste) {
		case copy:
			foreach(Item *item, mListSelectedItemsForPaste) {
				Item* newItem = item->clone();
				newItem->setPos(posCursor - topLeftSelection + item->scenePos());
				addItem(newItem);
				setZValue(newItem);
			}
			break;
		case cut:
			foreach(Item *item, mListSelectedItemsForPaste) {
				item->setPos(posCursor - topLeftSelection + item->scenePos());
				setZValue(item);
				mListSelectedItemsForPaste.clear();
			}
			break;
		default:
			break;
		}
	}
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

void Scene::addTextPicture(bool checked)
{
	if (checked)
		mItemType = textPicture;
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

void Scene::addImage(QString const &fileName)
{
	mItemType = image;
	mFileName = fileName;

	QSettings settings("SPbSU", "QReal");
	QString workingDirName = settings.value("workingDir", "./save").toString();
	QDir dir(workingDirName);
	dir.mkdir("images");
	mFileName = workingDirName + "/images/" + fileName.section('/', -1);
	QFile::copy(fileName, mFileName);
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
	mEmptyRect = addRect(0, 0, sizeEmptyRectX, sizeEmptyRectY, QPen(Qt::white));
	mZValue = 0;
}

bool Scene::compareItems(Item* first, Item* second)
{
	return first->zValue() < second->zValue();
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
	qSort(resList.begin(), resList.end(), compareItems);
	return resList;
}

QList<TextPicture *> Scene::selectedTextPictureItems()
{
	QList<TextPicture *> resList;
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		TextPicture* item = dynamic_cast<TextPicture*>(graphicsItem);
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

void Scene::changeFontPalette()
{
	mListSelectedTextPictureItems = selectedTextPictureItems();
	if (mListSelectedTextPictureItems.isEmpty())
		emit noSelectedTextPictureItems();
	else {
		TextPicture* item = dynamic_cast<TextPicture*>(mListSelectedTextPictureItems.back());
		if (item != NULL) {
			QPen penItem = item->pen();
			QFont fontItem = item->font();
			emit existSelectedTextPictureItems(penItem, fontItem, item->name());
		}
	}
}

void Scene::changeFontFamily(const QFont& font)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontFamily(font);
	update();
}

void Scene::changeFontPixelSize(int size)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontPixelSize(size);
	update();
}

void Scene::changeFontColor(const QString & text)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontColor(text);
	update();
}

void Scene::changeFontItalic(bool isChecked)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontItalic(isChecked);
	update();
}

void Scene::changeFontBold(bool isChecked)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontBold(isChecked);
	update();
}

void Scene::changeFontUnderline(bool isChecked)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setFontUnderline(isChecked);
	update();
}

void Scene::changeTextName(const QString &name)
{
	foreach (TextPicture *item, selectedTextPictureItems())
		item->setTextName(name);
	update();
}
