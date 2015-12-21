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

#include "curve.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>

#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/item/createItemPushButton.h"

using namespace qReal::shapeEdit;
using namespace qReal::commands;

Curve::Curve(const QPointF &start, const QPointF &end, const QPointF &c1)
		: Path(QPainterPath(start))
        , mCountClicks(0)
{
	setPen(QPen(Qt::gray));
	setBrush(QBrush(QColor(), Qt::NoBrush));
	mDomElementType = pictureType;
    init(start, end);
    mCurvePath->quadTo(c1, end);
}

Curve::Curve(const Curve &other)
		: Path(QPainterPath())
        , mCountClicks(0)
{
	mNeedScalingRect = other.mNeedScalingRect ;
	setPen(other.pen());
	setBrush(other.brush());
	mDomElementType = Item::pictureType;
	setX1(other.x1());
	setX2(other.x2());
	setY1(other.y1());
	setY2(other.y2());
	mC1 = other.mC1;
	mCurvePath = other.mCurvePath;
	mListScalePoint = other.mListScalePoint;
	setPos(other.x(), other.y());
}

void Curve::init(const QPointF &start, const QPointF &end)
{
    setX1(start.x());
    setY1(start.y());
    setX2(end.x());
    setY2(end.y());
    mCurvePath = new QPainterPath(start);
}

Item* Curve::clone()
{
	Curve* item = new Curve(*this);
	return item;
}

void Curve::mousePressEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    if (mCountClicks == 0) {
        mP1 = event->scenePos();
        mC1 = mP1;
        init(mP1, mP1);
        scene->setPenBrushForItem(this);
        scene->setZValue(this);
        scene->addItem(this);
    } else if (mCountClicks == 1) {
        mP2 = event->scenePos();
        init(mP1, mP2);
    } else if (mCountClicks == 2) {
        reshape(event);
        scene->setIsAddingFinished(true);
    }
    scene->removeMoveFlagForItem(event, nullptr);
}

void Curve::mouseMoveEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    Q_UNUSED(scene)
    if (mCountClicks == 1) {
        setX2(event->scenePos().x());
        setY2(event->scenePos().y());
        reshape(event);
    } else if (mCountClicks == 2) {
        reshape(event);
    }
}

void Curve::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    Q_UNUSED(event)
    Q_UNUSED(scene)
    ++mCountClicks;
}

void Curve::reshape(QGraphicsSceneMouseEvent *event)
{
    mC1 = event->scenePos();
    update();
}

void Curve::customizeButton(CreateItemPushButton *button) const
{
    button->setObjectName(QString("curvePushButton"));
    QIcon icon;
    icon.addFile(QString(":/mainWindow/images/curve.png"), QSize(), QIcon::Normal, QIcon::Off);
    button->setIcon(icon);
    button->setIconSize(QSize(45, 16));
    button->setToolTip(tr("Add a curve"));
}

QPainterPath Curve::shape() const
{
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);

	QPainterPath pathCtrl1;
	pathCtrl1.moveTo(x1(), y1());
	pathCtrl1.lineTo(mC1.x(), mC1.y());

	QPainterPath pathCtrl2;
	pathCtrl2.moveTo(x2(), y2());
	pathCtrl2.lineTo(mC1.x(), mC1.y());

	QPainterPathStroker ps;
	ps.setWidth(drift);
	pathCtrl1 = ps.createStroke(pathCtrl1);
	pathCtrl2 = ps.createStroke(pathCtrl2);
	path = ps.createStroke(*mCurvePath);

	path.addPath(pathCtrl1);
	path.addPath(pathCtrl2);

	return path;
}

QRectF Curve::searchMaxMinCoord() const
{
	qreal minX = qMin(qMin(x1(), x2()), mC1.x());
	qreal minY = qMin(qMin(y1(), y2()), mC1.y());
	qreal maxX = qMax(qMax(x1(), x2()), mC1.x());
	qreal maxY = qMax(qMax(y1(), y2()), mC1.y());
	return QRectF(minX, minY, maxX - minX, maxY - minY);
}

QRectF Curve::boundingRect() const
{
	return searchMaxMinCoord().adjusted(-drift, -drift, drift, drift);
}

void Curve::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	mCurvePath = new QPainterPath(QPointF(x1(), y1()));
	mCurvePath->quadTo(mC1, QPointF(x2(), y2()));
	painter->drawPath(*mCurvePath);
}

void Curve::drawExtractionForItem(QPainter* painter)
{
	painter->drawPoint(x1(), y1());
	painter->drawPoint(x2(), y2());
	painter->drawPoint(mC1.x(), mC1.y());

	setPenBrushDriftRect(painter);
	painter->drawPath(shape());
	drawFieldForResizeItem(painter);
}

void Curve::drawFieldForResizeItem(QPainter* painter)
{
	painter->drawEllipse(QPointF(x1(), y1()), resizeDrift, resizeDrift);
	painter->drawEllipse(QPointF(x2(), y2()), resizeDrift, resizeDrift);
	painter->drawEllipse(mC1, resizeDrift, resizeDrift);
}

void Curve::drawScalingRects(QPainter* painter)
{
	Q_UNUSED(painter);
}

void Curve::setCXandCY(qreal x, qreal y)
{
	mC1.setX(x);
	mC1.setY(y);
	update();
}

void Curve::changeDragState(qreal x, qreal y)
{
	Item::changeDragState(x, y);
	if (QRectF(QPointF(mC1.x() + scenePos().x(), mC1.y() + scenePos().y())
			, QSizeF(0, 0)).adjusted(-resizeDrift, -resizeDrift, resizeDrift, resizeDrift).contains(QPointF(x, y)))
	{
		setDragState(Ctrl);
	}
}

void  Curve::calcResizeItem(QGraphicsSceneMouseEvent *event)
{
	qreal x = mapFromScene(event->scenePos()).x();
	qreal y = mapFromScene(event->scenePos()).y();
	if (dragState() != None)
		setFlag(QGraphicsItem::ItemIsMovable, false);
	if (dragState() == TopLeft) {
		setX1(x);
		setY1(y);
	} else if (dragState() == BottomRight) {
		setX2(x);
		setY2(y);
	} else if (dragState() == Ctrl) {
		setCXandCY(x, y);
	}
}

QPair<QDomElement, Item::DomElementTypes> Curve::generateItem(QDomDocument &document, const QPoint &topLeftPicture)
{
	QDomElement curve = setPenBrushToDoc(document, "curve");

	const qreal x1 = scenePos().x() + this->x1() - topLeftPicture.x();
	const qreal y1 = scenePos().y() + this->y1()- topLeftPicture.y();
	const qreal x2 = scenePos().x() + this->x2() - topLeftPicture.x();
	const qreal y2 = scenePos().y() + this->y2() - topLeftPicture.y();
	const qreal x3 = scenePos().x() + mC1.x() - topLeftPicture.x();
	const qreal y3 = scenePos().y() + mC1.y() - topLeftPicture.y();

	QDomElement start  = document.createElement("start");
	curve.appendChild(start);
	start.setAttribute("starty", y1);
	start.setAttribute("startx", x1);

	QDomElement end  = document.createElement("end");
	curve.appendChild(end);
	end.setAttribute("endy", y2);
	end.setAttribute("endx", x2);

	QDomElement ctrl  = document.createElement("ctrl");
	curve.appendChild(ctrl);
	ctrl.setAttribute("y", y3);
	ctrl.setAttribute("x", x3);

	return QPair<QDomElement, Item::DomElementTypes>(curve, mDomElementType);
}
