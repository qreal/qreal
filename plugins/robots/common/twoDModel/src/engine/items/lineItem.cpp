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

#include <QtGui/QPainter>
#include <QtWidgets/QAction>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <qrkernel/settingsManager.h>

#include "lineItem.h"

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

LineItem::LineItem(const QPointF &begin, const QPointF &end, int cornerRadius)
	: mLineImpl()
	, mCornerRadius(cornerRadius)
{
	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());
	setPrivateData();
}

AbstractItem *LineItem::clone() const
{
	const auto cloned = new LineItem({x1(), y1()}, {x2(), y2()}, mCornerRadius);
	AbstractItem::copyTo(cloned);
	return cloned;
}

QAction *LineItem::lineTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_ruler.png"), tr("Line (L)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_L));
	return result;
}

void LineItem::setPrivateData()
{
	QPen pen(this->pen());
	pen.setColor(Qt::green);
	pen.setStyle(Qt::SolidLine);
	setPen(pen);
	mSerializeName = "line";
}

QRectF LineItem::boundingRect() const
{
	return mLineImpl.boundingRect(x1(), y1(), x2(), y2(), pen().width(), drift);
}

void LineItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mLineImpl.drawItem(painter, x1(), y1(), x2(), y2());
}

void LineItem::drawExtractionForItem(QPainter* painter)
{
	mLineImpl.drawPointExtractionForItem(painter, x1(), y1(), x2(), y2());
	setPenBrushDriftRect(painter);
	mLineImpl.drawExtractionForItem(painter, x1(), y1(), x2(), y2(), drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, x1(), y1(), x2(), y2());
}

QPainterPath LineItem::shape() const
{
	return mLineImpl.shape(drift, x1(), y1(), x2(), y2());
}

void LineItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	const bool isResizing = dragState() == TopLeft || dragState() == BottomRight;
	if (event->modifiers() & Qt::ShiftModifier && isResizing) {
		AbstractItem::resizeItem(event);
		reshapeRectWithShift();
	} else if (isResizing) {
		AbstractItem::resizeItem(event);
	} else {
		setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

void LineItem::reshapeRectWithShift()
{
	const qreal differenceX = qAbs(x2() - x1());
	const qreal differenceY = qAbs(y2() - y1());
	const qreal differenceXY = qAbs(differenceX - differenceY);
	const qreal size = qMax(differenceX, differenceY);
	const int delta = size / 2;
	if (differenceXY > delta) {
		const qreal corner1X = dragState() == TopLeft ? x2() : x1();
		const qreal corner1Y = dragState() == TopLeft ? y2() : y1();
		const qreal corner2X = dragState() == TopLeft ? x1() : x2();
		const qreal corner2Y = dragState() == TopLeft ? y1() : y2();
		const QPair<qreal, qreal> res = mLineImpl.reshapeRectWithShiftForLine(corner1X, corner1Y, corner2X, corner2Y
				, differenceX, differenceY, size);
		if (dragState() == TopLeft) {
			setX1(res.first);
			setY1(res.second);
		} else {
			setX2(res.first);
			setY2(res.second);
		}
	} else {
		const qreal size = qMax(qAbs(x2() - x1()), qAbs(y2() - y1()));
		if (dragState() == TopLeft) {
			setX1(x1() > x2() ? x2() + size : x2() - size);
			setY1(y1() > y2() ? y2() + size : y2() - size);
		} else {
			setX2(x2() > x1() ? x1() + size : x1() - size);
			setY2(y2() > y1() ? y1() + size : y1() - size);
		}
	}
}

QDomElement LineItem::serialize(QDomDocument &document, const QPointF &topLeftPicture) const
{
	QDomElement lineNode = setPenBrushToDoc(document, mSerializeName);
	AbstractItem::serialize(lineNode);
	mLineImpl.serialize(lineNode
			, x1() + scenePos().x() - topLeftPicture.x()
			, y1() + scenePos().y() - topLeftPicture.y()
			, x2() + scenePos().x() - topLeftPicture.x()
			, y2() + scenePos().y() - topLeftPicture.y());
	return lineNode;
}

void LineItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	const QPair<QPointF, QPointF> points = mLineImpl.deserialize(element);
	const QPointF begin = points.first;
	const QPointF end = points.second;

	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());

	deserializePenBrush(element);
}

void LineItem::deserializePenBrush(const QDomElement &element)
{
	readPenBrush(element);
}

void LineItem::setSerializeName(const QString &name)
{
	mSerializeName = name;
}
