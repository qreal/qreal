#include "text.h"

#include <QtGui/QCursor>
#include <QtGui/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>

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
	mText.setTextInteractionFlags(Qt::TextEditorInteraction);
	setPos(x, y);
	mText.setPlainText(text);
	mText.setParentItem(this);
	mBoundingRect = boundingRect();
	mX1 = x;
	mY1 = y;
}

void Text::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	mText.paint(painter, option, widget);
	if (mIsDynamicText)
		drawForDynamicText(painter);
}

void Text::drawExtractionForItem(QPainter* painter)
{
	mBoundingRect = boundingRect();
	mRect = mText.boundingRect();
	painter->drawPoint(mRect.left(), mRect.top());
	painter->drawPoint(mRect.left(), mRect.bottom());
	painter->drawPoint(mRect.right(), mRect.top());
	painter->drawPoint(mRect.right(), mRect.bottom());

	setPenBrushDriftRect(painter);
	painter->drawRect(mBoundingRect);
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
	qreal x1= mText.boundingRect().left();
	qreal y1 = mText.boundingRect().top();

	brush.setColor(mListScalePoint.at(4).second);
	painter->setBrush(brush);
	painter->drawRect(x1 - scalingRect, y1, scalingRect, scalingRect);

	brush.setColor(mListScalePoint.at(0).second);
	painter->setBrush(brush);
	painter->drawRect(x1, y1 - scalingRect, scalingRect, scalingRect);
}

QRectF Text::boundingRect() const
{
	return mText.boundingRect().adjusted(-drift, -drift, drift, drift);
}

QRectF Text::realBoundingRect() const
{
	return mapToScene(mText.boundingRect()).boundingRect();
}

void Text::drawForDynamicText(QPainter* painter)
{
	QPen pen(Qt::green);
	pen.setWidth(2);
	painter->setPen(pen);
	painter->drawRect(mText.boundingRect());
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
	qreal x1= mText.boundingRect().left();
	qreal x2 = mText.boundingRect().right();
	qreal y1 = mText.boundingRect().top();
	qreal y2 = mText.boundingRect().bottom();
	int correction = 0;
	calcForChangeScalingState(QPointF(x, y), QPointF(x1, y1), QPointF(x2, y2), correction);
}

QPair<QDomElement, Item::DomElementTypes> Text::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement text = document.createElement("label");
	int const x1 = static_cast<int>(realBoundingRect().left() - topLeftPicture.x());
	int const y1 = static_cast<int>(realBoundingRect().top() - topLeftPicture.y());
	text.setAttribute("y", setSingleScaleForDoc(4, x1, y1));
	text.setAttribute("x", setSingleScaleForDoc(0, x1, y1));
	text.setAttribute(mIsDynamicText ? "textBinded" : "text", mText.toPlainText());

	return QPair<QDomElement, Item::DomElementTypes>(text, mDomElementType);
}

void Text::setIsDynamicText(bool isDynamic)
{
	mIsDynamicText = isDynamic;
}

void Text::setZValueAll(int const &index)
{
	mText.setZValue(index);
	setZValue(index);
}
