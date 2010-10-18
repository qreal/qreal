#include "text.h"

#include <QtGui/QCursor>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QTextCursor>

Text::Text(bool isDynamic)
	: Item(NULL), mIsDynamicText(isDynamic)
{
	mDomElementType = labelType;
}

Text::Text(int x, int y, QString const &text, bool isDynamic)
	: Item(NULL), mIsDynamicText(isDynamic)
{
	mDomElementType = labelType;
	init(x, y, text);
}

void Text::init(int x, int y, QString const &text)
{
	mNeedScalingRect = false;
	mText.setPlainText(text);
	mText.setParentItem(this);
	mText.setPos(x, y);
	mBoundingRect = QRectF(x, y, 0, 0);
	mText.setTextInteractionFlags(Qt::TextEditorInteraction);
	mX1 = x;
	mY1 = y;
}

Text::Text(Text const &other)
	:Item()
{
	mIsDynamicText = other.mIsDynamicText;
	mNeedScalingRect = other.mNeedScalingRect;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = labelType;
	mX1 = other.mX1;
	mY1 = other.mY1;
	mText.setPos(other.mText.x(), other.mText.y());
	mText.setFlags(other.mText.flags());
	mText.setTextInteractionFlags(Qt::TextEditorInteraction);
	mText.setPlainText(other.mText.toPlainText());
	mText.setParentItem(this);
	mBoundingRect = other.mBoundingRect;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

Item* Text::clone()
{
	Text* item = new Text(*this);
	return item;
}

void Text::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mBoundingRect = mText.boundingRect().adjusted(mX1, mY1, mX1, mY1);
	if (mIsDynamicText)
		drawForDynamicText(painter);
}

void Text::drawExtractionForItem(QPainter* painter)
{
	QRectF rect = mBoundingRect;
	painter->drawPoint(rect.left(), rect.top());
	painter->drawPoint(rect.left(), rect.bottom());
	painter->drawPoint(rect.right(), rect.top());
	painter->drawPoint(rect.right(), rect.bottom());

	setPenBrushDriftRect(painter);
	painter->drawRect(boundingRect());
	drawFieldForResizeItem(painter);

	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::SolidPattern));
	drawScalingRects(painter);
}

void Text::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Text::drawScalingRects(QPainter* painter)
{
	QBrush brush(Qt::SolidPattern);
	QRectF rect = mBoundingRect;
	qreal x1= rect.left();
	qreal y1 = rect.top();

	brush.setColor(mListScalePoint.at(4).second);
	painter->setBrush(brush);
	painter->drawRect(x1 - scalingRect, y1, scalingRect, scalingRect);

	brush.setColor(mListScalePoint.at(0).second);
	painter->setBrush(brush);
	painter->drawRect(x1, y1 - scalingRect, scalingRect, scalingRect);
}

QRectF Text::boundingRect() const
{
	return mBoundingRect.adjusted(-drift, -drift, drift, drift);
}

QRectF Text::realBoundingRect() const
{
	return mapToScene(mBoundingRect).boundingRect();
}

void Text::drawForDynamicText(QPainter* painter)
{
	QPen pen(Qt::green);
	pen.setWidth(2);
	painter->setPen(pen);
	painter->drawRect(mBoundingRect);
}

bool Text::isDynamicText()
{
	return mIsDynamicText;
}

QGraphicsTextItem const& Text::getText()
{
	return mText;
}

void Text::changeScalingPointState(qreal x, qreal y)
{
	QRectF rect = mBoundingRect;
	qreal x1= rect.left();
	qreal x2 = rect.right();
	qreal y1 = rect.top();
	qreal y2 = rect.bottom();
	int correction = 0;
	calcForChangeScalingState(QPointF(x, y), QPointF(x1, y1), QPointF(x2, y2), correction);
}

QPair<QDomElement, Item::DomElementTypes> Text::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement text = document.createElement("label");
	int const x1 = static_cast<int>(mapToScene(mBoundingRect).boundingRect().left() - topLeftPicture.x());
	int const y1 = static_cast<int>(mapToScene(mBoundingRect).boundingRect().top() - topLeftPicture.y());
	text.setAttribute("y", setSingleScaleForDoc(4, x1, y1));
	text.setAttribute("x", setSingleScaleForDoc(0, x1, y1));
	text.setAttribute(mIsDynamicText ? "textBinded" : "text", mText.toPlainText());

	return QPair<QDomElement, Item::DomElementTypes>(text, mDomElementType);
}

void Text::setIsDynamicText(bool isDynamic)
{
	mIsDynamicText = isDynamic;
}

void Text::setItemZValue(int zValue)
{
	mZValue = zValue;
	mText.setZValue(zValue);
	setZValue(zValue);
}

void Text::focusInEvent(QFocusEvent *event)
{
	Q_UNUSED(event);
	mText.setCursor(cursor());
	mText.setTextInteractionFlags(Qt::TextEditorInteraction);
}

void Text::focusOutEvent(QFocusEvent *event)
{
	Q_UNUSED(event);
	mText.setTextInteractionFlags(Qt::NoTextInteraction);
	QTextCursor cursor = mText.textCursor();
	cursor.clearSelection();
	mText.setTextCursor(cursor);
	mText.unsetCursor();
}
