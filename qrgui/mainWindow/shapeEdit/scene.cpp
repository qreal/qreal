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

#include "scene.h"

#include <limits>
#include <QtCore/QPointF>
#include <QtGui/QKeyEvent>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>

using namespace qReal;

Scene::Scene(graphicsUtils::AbstractView *view, QObject * parent)
	:  AbstractScene(view, parent)
	, mItemType(none)
	, mWaitMove(false)
	, mCount(0)
	, mSelectedTextPicture(nullptr)
{
	mSizeEmptyRectX = sizeEmptyRectX;
	mSizeEmptyRectY = sizeEmptyRectY;
	setItemIndexMethod(NoIndex);
	setEmptyRect(0, 0, mSizeEmptyRectX, mSizeEmptyRectY);
	setEmptyPenBrushItems();
	mCopyPaste = nonePaste;
	mPortType = "NonTyped";

	connect(this, SIGNAL(selectionChanged()), this, SLOT(changePalette()));
	connect(this, SIGNAL(selectionChanged()), this, SLOT(changeFontPalette()));
	connect(this, SIGNAL(selectionChanged()), this, SLOT(changePortsComboBox()));

	mZValue = 0;
}

QRectF Scene::selectedItemsBoundingRect() const
{
	QRectF resBoundRect;
	QList<Item *> list = mListSelectedItemsForPaste;
	foreach (Item *graphicsItem, list)
		resBoundRect |= graphicsItem->sceneBoundingRect();
	return resBoundRect;
}

QPoint Scene::centerEmpty()
{
	return QPoint(sizeEmptyRectX / 2, sizeEmptyRectY / 2);
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
	mLine->setX2(mX2);
	mLine->setY2(mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mLine->reshapeRectWithShift();
}

void Scene::reshapeLinePort(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mLinePort->setX2(mX2);
	mLinePort->setY2(mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mLinePort->reshapeRectWithShift();
}

void Scene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mEllipse->setX2(mX2);
	mEllipse->setY2(mY2);
	if (event->modifiers() & Qt::ShiftModifier)
		mEllipse->reshapeRectWithShift();
}

void Scene::reshapeRectangle(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mRectangle->setX2(mX2);
	mRectangle->setY2(mY2);
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
	mCurve->setX2(mX2);
	mCurve->setY2(mY2);
}

void Scene::reshapeCurveSecond(QGraphicsSceneMouseEvent *event)
{
	mC1 = setCXandCY(event);
	mCurve->setCXandCY(mC1.x(), mC1.y());
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
		if (item->dragState() != Item::None)  {
			mView->setDragMode(QGraphicsView::NoDrag);
			return QPair<bool, Item *>(true, item);
		}
	}
	return QPair<bool, Item *>(false, nullptr);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	setDragMode(mItemType);
	switch (mItemType) {
	case curve:
		if (mCount == 1)
			setX1andY1(event);
		else if (mCount == 2)
			reshapeCurveFirst(event);
		else if (mCount == 3)
			reshapeCurveSecond(event);
		removeMoveFlag(event, nullptr);
		mCount++;
		break;
	case stylus :
		setX1andY1(event);
		mStylus = new Stylus(mX1, mY1, nullptr);
		mStylus->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
		addItem(mStylus);
		setZValue(mStylus);
		removeMoveFlag(event, mStylus);
		mWaitMove = true;
		break;
	case line :
		setX1andY1(event);
		mLine = new Line(mX1, mY1, mX1, mY1, nullptr);
		mLine->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
		addItem(mLine);
		setZValue(mLine);
		removeMoveFlag(event, mLine);
		mWaitMove = true;
		break;
	case ellipse :
		setX1andY1(event);
		mEllipse = new QRealEllipse(mX1, mY1, mX1, mY1, nullptr);
		mEllipse->setPenBrush(mPenStyleItems, mPenWidthItems, mPenColorItems, mBrushStyleItems, mBrushColorItems);
		addItem(mEllipse);
		setZValue(mEllipse);
		removeMoveFlag(event, mEllipse);
		mWaitMove = true;
		break;
	case rectangle :
		setX1andY1(event);
		mRectangle = new QRealRectangle(mX1, mY1, mX1, mY1, nullptr);
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
		mPointPort = new PointPort(mX1, mY1, nullptr);
		mPointPort->setType(mPortType);
		addItem(mPointPort);
		setZValue(mPointPort);
		break;
	case linePort :
		setX1andY1(event);
		mLinePort = new LinePort(mX1, mY1, mX1, mY1, nullptr);
		mLinePort->setType(mPortType);
		addItem(mLinePort);
		setZValue(mLinePort);
		removeMoveFlag(event, mLinePort);
		mWaitMove = true;
		break;
	case image :
		setX1andY1(event);
		mImage = new Image(mFileName, mX1, mY1, nullptr);
		addItem(mImage);
		setZValue(mImage);
		break;
	default:  // if we wait some resize
		setX1andY1(event);
		mGraphicsItem = dynamic_cast<graphicsUtils::AbstractItem *>(itemAt(event->scenePos(), QTransform()));
		if (mGraphicsItem != nullptr) {
			mGraphicsItem->changeDragState(mX1, mY1);
			Item *graphicsItem = dynamic_cast<Item *>(mGraphicsItem);
			graphicsItem->changeScalingPointState(mX1, mY1);
			forPressResize(event);
			if (graphicsItem->getScalingPointState() != Item::noneScale) {
				graphicsItem->setScalingPointColor();
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
	setDragMode(mItemType);
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
			if (mCurve != nullptr)
				reshapeCurveFirst(event);
		} else if (mCount == 3)
			reshapeCurveSecond(event);
		break;
	case linePort :
		if (mWaitMove)
			reshapeLinePort(event);
		break;
	default:  // if we wait some resize
		forMoveResize(event);
		break;
	}
	update();
}

void Scene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	setDragMode(mItemType);
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
		forReleaseResize(event);
		break;
	}
	mWaitMove = false;
	setMoveFlag(event);
	setNullZValueItems();
	if (mItemType != curve || mCount > 3) {
		emit resetHighlightAllButtons();
		mItemType = none;
		mCount = 0;
		mCurve = nullptr;
	}
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
		mCurve = nullptr;
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

void Scene::addImage(const QString &fileName)
{
	mItemType = image;
	mFileName = fileName;

	QString workingDirName = QFileInfo(QApplication::applicationFilePath()).absoluteDir().absolutePath();
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

QList<Item *> Scene::selectedSceneItems()
{
	QList<Item *> resList;
	mListSelectedItems = selectedItems();
	foreach (QGraphicsItem *graphicsItem, mListSelectedItems) {
		Item* item = dynamic_cast<Item*>(graphicsItem);
		if (item != nullptr)
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
		if (item != nullptr)
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

void Scene::changePortsType(const QString &type)
{
	mPortType = type;

	foreach (QGraphicsItem *item, selectedItems()) {
		PointPort *point = dynamic_cast<PointPort *>(item);
		if (point) {
			point->setType(type);
		} else {
			LinePort *line = dynamic_cast<LinePort *>(item);
			if (line) {
				line->setType(type);
			}
		}
	}

	update();
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
			if (item != nullptr) {
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
		if (item != nullptr) {
			QPen penItem = item->pen();
			QFont fontItem = item->font();
			emit existSelectedTextPictureItems(penItem, fontItem, item->name());
		}
	}
}

void Scene::changePortsComboBox()
{
	foreach (QGraphicsItem *item, selectedItems()) {
		PointPort *point = dynamic_cast<PointPort *>(item);
		if (point) {
			emit existSelectedPortItems(point->getType());
			return;
		}
		LinePort *line = dynamic_cast<LinePort *>(item);
		if (line) {
			emit existSelectedPortItems(line->getType());
			return;
		}
	}
	emit noSelectedPortItems();
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

void Scene::changeFontColor(const QString &text)
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
