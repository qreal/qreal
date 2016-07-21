/* Copyright 2015 CyberTech Labs Ltd.
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

#include "curveItem.h"

#include <QtGui/QPainter>
#include <QtWidgets/QAction>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <qrkernel/settingsManager.h>
#include <qrutils/mathUtils/math.h>

using namespace twoDModel::items;
using namespace qReal;
using namespace graphicsUtils;

const QSizeF markerSize(12, 12);
const QColor markerColor1 = "#ffcc66";
const QColor markerColor2 = "#ff6666";

CurveItem::CurveItem(const QPointF &begin, const QPointF &end)
	: mMarker1(this)
	, mMarker2(this)
{
	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());
	mMarker1.setPos(begin + QPointF(-30, 0));
	mMarker2.setPos(end + QPointF(130, 0));
	mMarker1.setZValue(1);
	mMarker2.setZValue(1);
	mMarker1.setVisible(isSelected());
	mMarker2.setVisible(isSelected());
	setPrivateData();
}

AbstractItem *CurveItem::clone() const
{
	const auto cloned = new CurveItem({x1(), y1()}, {x2(), y2()});
	AbstractItem::copyTo(cloned);
	connect(&mMarker1, &Marker::xChanged, &cloned->mMarker1, [=]() { cloned->mMarker1.setX(mMarker1.x()); });
	connect(&mMarker1, &Marker::yChanged, &cloned->mMarker1, [=]() { cloned->mMarker1.setY(mMarker1.y()); });
	connect(&mMarker2, &Marker::xChanged, &cloned->mMarker2, [=]() { cloned->mMarker2.setX(mMarker2.x()); });
	connect(&mMarker2, &Marker::yChanged, &cloned->mMarker2, [=]() { cloned->mMarker2.setY(mMarker2.y()); });
	cloned->mMarker1.setPos(mMarker1.pos());
	cloned->mMarker2.setPos(mMarker2.pos());
	return cloned;
}

QAction *CurveItem::curveTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_bezier.svg"), tr("Bezier Curve (B)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_B));
	return result;
}

void CurveItem::setPrivateData()
{
	QPen pen(this->pen());
	pen.setColor(Qt::green);
	pen.setStyle(Qt::SolidLine);
	setPen(pen);
}

QRectF CurveItem::boundingRect() const
{
	const qreal penWidth = pen().width();
	const qreal minX = qMin(qMin(x1(), x2()), qMin(mMarker1.x(), mMarker2.x()));
	const qreal minY = qMin(qMin(y1(), y2()), qMin(mMarker1.y(), mMarker2.y()));
	const qreal maxX = qMax(qMax(x1(), x2()), qMax(mMarker1.x(), mMarker2.x()));
	const qreal maxY = qMax(qMax(y1(), y2()), qMax(mMarker1.y(), mMarker2.y()));
	return QRectF(minX, minY, maxX - minX, maxY - minY)
			.adjusted(-drift - penWidth, -drift - penWidth, drift + penWidth, drift + penWidth);
}

void CurveItem::drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawPath(shape());
}

void CurveItem::drawExtractionForItem(QPainter *painter)
{
	painter->drawPoint(x1(), y1());
	painter->drawPoint(x2(), y2());
	setPenBrushDriftRect(painter);
	painter->drawEllipse(QPointF(x1(), y1()), resizeDrift, resizeDrift);
	painter->drawEllipse(QPointF(x2(), y2()), resizeDrift, resizeDrift);
	painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));
	painter->drawLine(x1(), y1(), mMarker1.x(), mMarker1.y());
	painter->drawLine(x2(), y2(), mMarker2.x(), mMarker2.y());
}

QPainterPath CurveItem::shape() const
{
	QPainterPath path;
	path.moveTo(x1(), y1());
	path.cubicTo(mMarker1.x(), mMarker1.y(), mMarker2.x(), mMarker2.y(), x2(), y2());
	return path;
}

void CurveItem::resizeItem(QGraphicsSceneMouseEvent *event)
{
	if (event->modifiers() & Qt::ShiftModifier) {
		setX2(event->scenePos().x());
		setY2(event->scenePos().y());
		reshapeRectWithShift();
	} else if (dragState() == TopLeft || dragState() == BottomRight) {
		AbstractItem::resizeItem(event);
	} else {
		setFlag(QGraphicsItem::ItemIsMovable, true);
	}
}

QDomElement CurveItem::serialize(QDomDocument &document, const QPointF &topLeftPicture) const
{
	QDomElement curveNode = setPenBrushToDoc(document, "cubicBezier");
	AbstractItem::serialize(curveNode);
	const qreal x1 = this->x1() + scenePos().x() - topLeftPicture.x();
	const qreal y1 = this->y1() + scenePos().y() - topLeftPicture.y();
	const qreal x2 = this->x2() + scenePos().x() - topLeftPicture.x();
	const qreal y2 = this->y2() + scenePos().y() - topLeftPicture.y();
	const QPointF cp1 = mMarker1.pos() + scenePos() - topLeftPicture;
	const QPointF cp2 = mMarker2.pos() + scenePos() - topLeftPicture;
	curveNode.setAttribute("begin", QString::number(x1) + ":" + QString::number(y1));
	curveNode.setAttribute("end", QString::number(x2) + ":" + QString::number(y2));
	curveNode.setAttribute("cp1", QString::number(cp1.x()) + ":" + QString::number(cp1.y()));
	curveNode.setAttribute("cp2", QString::number(cp2.x()) + ":" + QString::number(cp2.y()));
	return curveNode;
}

void CurveItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	const QPointF begin = deserializePoint(element.attribute("begin"));
	const QPointF end = deserializePoint(element.attribute("end"));
	const QPointF cp1 = deserializePoint(element.attribute("cp1"));
	const QPointF cp2 = deserializePoint(element.attribute("cp2"));

	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());
	mMarker1.setPos(mapFromScene(cp1));
	mMarker2.setPos(mapFromScene(cp2));

	deserializePenBrush(element);
}

void CurveItem::deserializePenBrush(const QDomElement &element)
{
	readPenBrush(element);
}

QPointF CurveItem::deserializePoint(const QString &string) const
{
	const QStringList splittedStr = string.split(":");
	if (splittedStr.count() == 2) {
		const qreal x = splittedStr[0].toDouble();
		const qreal y = splittedStr[1].toDouble();
		return QPointF(x, y);
	}

	return QPointF();
}

QVariant CurveItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSelectedHasChanged) {
		mMarker1.setVisible(value.toBool());
		mMarker2.setVisible(value.toBool());
	}

	return ColorFieldItem::itemChange(change, value);
}

CurveItem::Marker::Marker(QGraphicsItem *parent)
	: QGraphicsObject(parent)
{
	setFlag(ItemSendsGeometryChanges);
}

QRectF CurveItem::Marker::boundingRect() const
{
	return QRectF(-markerSize.width() / 2, -markerSize.height() / 2, markerSize.width(), markerSize.height());
}

void CurveItem::Marker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->save();
	painter->setPen(markerColor1);
	painter->setBrush(markerColor1);
	painter->drawEllipse(boundingRect());
	painter->setPen(markerColor2);
	painter->setBrush(markerColor2);
	painter->drawEllipse(QRectF(-1, -1, 2, 2));
	painter->restore();
}

QVariant CurveItem::Marker::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionChange) {
		parentItem()->update();
	}

	return QGraphicsItem::itemChange(change, value);
}

void CurveItem::Marker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
}

void CurveItem::Marker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
	setPos(parentItem()->mapFromScene(event->scenePos()));
}

void CurveItem::Marker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	Q_UNUSED(event)
}
