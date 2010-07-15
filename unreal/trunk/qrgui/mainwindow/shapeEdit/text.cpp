#include "text.h"

#include <QtGui/QCursor>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

Text::Text(qreal x, qreal y, QString const &text, bool isDynamic)
	: Item(NULL), mIsDynamicText(isDynamic)
{
	mDomElementType = labelType;
	mText.setTextInteractionFlags(Qt::TextEditorInteraction);
	setPos(x, y);
	mText.setHtml(text);
	mText.setParentItem(this);
}

void Text::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	mText.paint(painter, option, widget);
	if (mIsDynamicText)
		drawForDynamicText(painter);
}

void Text::drawExtractionForItem(QPainter* painter)
{
	mRect = mText.boundingRect();
	painter->drawPoint(mRect.left(), mRect.top());
	painter->drawPoint(mRect.left(), mRect.bottom());
	painter->drawPoint(mRect.right(), mRect.top());
	painter->drawPoint(mRect.right(), mRect.bottom());
}

QRectF Text::boundingRect() const
{
	return mText.boundingRect().adjusted(-10, -10, 10, 10);
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

QPair<QDomElement, Item::DomElementTypes> Text::generateItem(QDomDocument &document, QPointF const &topLeftPicture)
{
	QDomElement text = document.createElement("label");
	qreal const x1 = mText.boundingRect().x() + scenePos().x() - topLeftPicture.x();
	qreal const y1 = mText.boundingRect().y() + scenePos().y() - topLeftPicture.y();
	text.setAttribute("y", y1);
	text.setAttribute("x", x1);
	text.setAttribute(mIsDynamicText ? "textBinded" : "text", mText.toPlainText());

	return QPair<QDomElement, Item::DomElementTypes>(text, mDomElementType);
}

void Text::setIsDynamicText(bool isDynamic)
{
	mIsDynamicText = isDynamic;
}
