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

#include "ellipseItem.h"

#include <QtWidgets/QAction>

using namespace twoDModel::items;
using namespace graphicsUtils;

const int currentResizeDrift = resizeDrift;
const int currentDrift = drift / 2;

EllipseItem::EllipseItem(const QPointF &begin, const QPointF &end)
	: mEllipseImpl()
{
	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());
	setPrivateData();
}

AbstractItem *EllipseItem::clone() const
{
	EllipseItem * const cloned = new EllipseItem({x1(), y1()}, {x2(), y2()});
	AbstractItem::copyTo(cloned);
	return cloned;
}

QAction *EllipseItem::ellipseTool()
{
	QAction * const result = new QAction(QIcon(":/icons/2d_ellipse.png"), tr("Ellipse (E)"), nullptr);
	result->setShortcut(QKeySequence(Qt::Key_E));
	return result;
}

void EllipseItem::setPrivateData()
{
	QPen pen(this->pen());
	pen.setColor(Qt::blue);
	pen.setStyle(Qt::SolidLine);
	setPen(pen);
}

QRectF EllipseItem::calcNecessaryBoundingRect() const
{
	return QRectF(qMin(x1(), x2()), qMin(y1(), y2()), abs(x2() - x1()), abs(y2() - y1()));
}

QRectF EllipseItem::boundingRect() const
{
	return mEllipseImpl.boundingRect(x1(), y1(), x2(), y2(), currentDrift);
}

void EllipseItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mEllipseImpl.drawEllipseItem(painter, x1(), y1(), x2(), y2());
}

QDomElement EllipseItem::serialize(QDomDocument &document, const QPointF &topLeftPicture) const
{
	QDomElement ellipseNode = setPenBrushToDoc(document, "ellipse");
	AbstractItem::serialize(ellipseNode);
	ellipseNode.setAttribute("begin", QString::number(x1() + scenePos().x() - topLeftPicture.x())
			 + ":" + QString::number(y1() + scenePos().y() - topLeftPicture.y()));
	ellipseNode.setAttribute("end", QString::number(x2() + scenePos().x() - topLeftPicture.x())
			 + ":" + QString::number(y2() + scenePos().y() - topLeftPicture.y()));
	return ellipseNode;
}

void EllipseItem::deserialize(const QDomElement &element)
{
	AbstractItem::deserialize(element);
	const QString beginStr = element.attribute("begin", "0:0");
	QStringList splittedStr = beginStr.split(":");
	int x = splittedStr[0].toInt();
	int y = splittedStr[1].toInt();
	const QPointF begin = QPointF(x, y);

	const QString endStr = element.attribute("end", "0:0");
	splittedStr = endStr.split(":");
	x = splittedStr[0].toInt();
	y = splittedStr[1].toInt();
	const QPointF end = QPointF(x, y);

	setX1(begin.x());
	setY1(begin.y());
	setX2(end.x());
	setY2(end.y());
	readPenBrush(element);
}

QPainterPath EllipseItem::shape() const
{
	QPainterPath result;
	result.addEllipse(boundingRect());
	return result;
}

bool EllipseItem::filled() const
{
	return brush().style() != Qt::NoBrush;
}

void EllipseItem::setFilled(bool filled)
{
	setBrushStyle(filled ? "Solid" : "None");
	update();
}
