#include "stylus.h"

Stylus::Stylus(qreal x1, qreal y1, Item* parent):Item(parent)
{
	mPen.setColor(Qt::black);
	mX1 = x1;
	mY1 = y1;
	mTmpX1 = x1;
	mTmpY1 = y1;
	mDomElementType = pictureType;
}

void Stylus::addLine(qreal x2, qreal y2)
{
	mX2 = x2;
	mY2 = y2;
	Line *line = new Line(mTmpX1, mTmpY1, mX2, mY2, NULL);
	line->setPen(mPen);
	line->setBrush(mBrush);
	mListLine.push_back(line);
	mTmpX1 = mX2;
	mTmpY1 = mY2;
}

QRectF Stylus::searchMaxMinCoord() const
{
	if(!mListLine.isEmpty()) {
		qreal maxX = (mListLine.at(0))->boundingRect().right();
		qreal minX = (mListLine.at(0))->boundingRect().left();
		qreal maxY = (mListLine.at(0))->boundingRect().bottom();
		qreal minY = (mListLine.at(0))->boundingRect().top();
		foreach (Line *line, mListLine) {
			minX = qMin(line->boundingRect().left(), minX);
			minY = qMin(line->boundingRect().top(), minY);
			maxX = qMax(line->boundingRect().right(), maxX);
			maxY = qMax(line->boundingRect().bottom(), maxY);
		}
		return QRectF(minX, minY, maxX - minX, maxY - minY);
	}
	else
		return QRectF(0, 0, 0, 0);
}

QPainterPath Stylus::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	foreach (Line *line, mListLine) {
		path.addPath(line->shape());
	}
	return path;
}

QRectF Stylus::boundingRect() const
{
	return searchMaxMinCoord();
}

void Stylus::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	foreach (Line *line, mListLine) {
		painter->setPen(line->pen());
		painter->setBrush(line->brush());
		line->drawItem(painter, option, widget);
	}
}

void Stylus::drawExtractionForItem(QPainter* painter)
{
	mRect = boundingRect();
	painter->drawPoint(mRect.left(), mRect.top());
	painter->drawPoint(mRect.left(), mRect.bottom());
	painter->drawPoint(mRect.right(), mRect.top());
	painter->drawPoint(mRect.right(), mRect.bottom());

	/*setPenBrushDriftRect(painter);
	painter->drawPath(shape());
	drawFieldForResizeItem(painter);*/
}

void Stylus::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}

QPair<QDomElement, Item::DomElementTypes> Stylus::generateItem(QDomDocument &document, QPointF const &topLeftPicture)
{
	QDomElement stylus = document.createElement("stylus");
	foreach (Line *line, mListLine) {
		QDomElement item = (line->generateItem(document, topLeftPicture)).first;
		stylus.appendChild(item);
	}
	return QPair<QDomElement, Item::DomElementTypes>(stylus, mDomElementType);
}
