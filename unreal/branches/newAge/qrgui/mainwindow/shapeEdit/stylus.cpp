#include "stylus.h"

Stylus::Stylus(qreal x1, qreal y1, Item* parent):Item(parent)
{
	mNeedScalingRect = false;
	mPen.setColor(Qt::black);
	mX1 = x1;
	mY1 = y1;
	mTmpX1 = x1;
	mTmpY1 = y1;
	mDomElementType = pictureType;
}

Stylus::Stylus(Stylus const &other)
	:Item()
{
	mNeedScalingRect = other.mNeedScalingRect ;
	mPen = other.mPen;
	mBrush = other.mBrush;
	mDomElementType = pictureType;
	mX1 = other.mX1;
	mX2 = other.mX2;
	mY1 = other.mY1;
	mY2 = other.mY2;
	mTmpX1 = other.mTmpX1;
	mTmpY1 = other.mTmpY1;
	mListScalePoint = other.mListScalePoint;
	foreach (Line *line, other.mListLine) {
		Line *newLine = new Line(*line);
		mListLine.append(newLine);
	}
	setPos(other.x(), other.y());
}

Item* Stylus::clone()
{
	Stylus* item = new Stylus(*this);
	return item;
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
		qreal maxX = (mListLine.at(0))->realBoundingRect().right();
		qreal minX = (mListLine.at(0))->realBoundingRect().left();
		qreal maxY = (mListLine.at(0))->realBoundingRect().bottom();
		qreal minY = (mListLine.at(0))->realBoundingRect().top();
		foreach (Line *line, mListLine) {
			minX = qMin(line->realBoundingRect().left(), minX);
			minY = qMin(line->realBoundingRect().top(), minY);
			maxX = qMax(line->realBoundingRect().right(), maxX);
			maxY = qMax(line->realBoundingRect().bottom(), maxY);
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
	QRectF rect = boundingRect();
	painter->drawPoint(rect.left(), rect.top());
	painter->drawPoint(rect.left(), rect.bottom());
	painter->drawPoint(rect.right(), rect.top());
	painter->drawPoint(rect.right(), rect.bottom());

	/*setPenBrushDriftRect(painter);
	painter->drawPath(shape());
	drawFieldForResizeItem(painter);*/
}

void Stylus::drawFieldForResizeItem(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Stylus::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Stylus::setPenStyle(const QString& text)
{
	Item::setPenStyle(text);
	foreach (Line *line, mListLine)
		line->setPenStyle(text);
}

void Stylus::setPenWidth(int width)
{
	Item::setPenWidth(width);
	foreach (Line *line, mListLine)
		line->setPenWidth(width);
}

void Stylus::setPenColor(const QString& text)
{
	Item::setPenColor(text);
	foreach (Line *line, mListLine)
		line->setPenColor(text);
}

void Stylus::setBrushStyle(const QString& text)
{
	Item::setBrushStyle(text);
	foreach (Line *line, mListLine)
		line->setBrushStyle(text);
}

void Stylus::setBrushColor(const QString& text)
{
	Item::setBrushColor(text);
	foreach (Line *line, mListLine)
		line->setBrushColor(text);
}

QPair<QDomElement, Item::DomElementTypes> Stylus::generateItem(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement stylus = document.createElement("stylus");
	foreach (Line *line, mListLine) {
		QDomElement item = (line->generateItem(document, topLeftPicture - QPoint(static_cast<int>(scenePos().x()), static_cast<int>(scenePos().y())))).first;
		stylus.appendChild(item);
	}
	return QPair<QDomElement, Item::DomElementTypes>(stylus, mDomElementType);
}
