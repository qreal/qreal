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

#include "item.h"

#include <math.h>
#include <QtGui/QPainter>
#include <QtWidgets/QStyle>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtCore/QList>

#include <qrkernel/settingsManager.h>

#include "mainWindow/shapeEdit/item/createItemPushButton.h"
#include "mainWindow/shapeEdit/scene.h"

using namespace qReal;
using namespace qReal::shapeEdit;
using namespace qReal::commands;

Item::Item(graphicsUtils::AbstractItem* parent)
	: AbstractItem(parent), mDomElementType(noneType)
	, mScalingState(noneScale)
{
	mNeedScalingRect = false;
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	QBrush brush(this->brush());
	brush.setColor(pen().color());
	setBrush(brush);
	mZValue = 0;
	initListScalePoint();
}

void Item::setItemZValue(int zValue)
{
	mZValue = zValue;
	setZValue(zValue);
}

int Item::itemZValue() const
{
	return mZValue;
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    Q_UNUSED(event)
    Q_UNUSED(scene)
}

void Item::mouseMoveEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    if (scene->getWaitMove()) {
        reshape(event);
    }
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, Scene *scene)
{
    reshape(event);
    scene->setIsAddingFinished(true);
}

void Item::reshape(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
}

void Item::initListScalePoint()
{
	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(topLeftX, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(topRightX, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(bottomLeftX, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(bottomRightX, QColor(Qt::black)));

	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(topLeftY, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(topRightY, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(bottomLeftY, QColor(Qt::black)));
	mListScalePoint.push_back(QPair<ScalingPointState, QColor>(bottomRightY, QColor(Qt::black)));
}

QRectF Item::calcNecessaryBoundingRect() const
{
	if (mNeedScalingRect)
		return boundingRect().adjusted(scalingDrift, scalingDrift, -scalingDrift, -scalingDrift);
	else
		return boundingRect();
}

void Item::drawExtractionForItem(QPainter* painter)
{
	AbstractItem::drawExtractionForItem(painter);

	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::SolidPattern));
	drawScalingRects(painter);
}

void Item::drawScalingRects(QPainter* painter)
{
	QRectF itemBoundingRect = calcNecessaryBoundingRect();
	qreal x1= itemBoundingRect.left();
	qreal x2 = itemBoundingRect.right();
	qreal y1 = itemBoundingRect.top();
	qreal y2 = itemBoundingRect.bottom();

	for (int i = 0; i < mListScalePoint.size(); ++i) {
		QPair<Item::ScalingPointState, QColor> point = mListScalePoint.at(i);
		QBrush brush(Qt::SolidPattern);
		brush.setColor(point.second);
		painter->setBrush(brush);
		if (point.first == Item::topLeftX) {
			painter->drawRect(x1, y1 - scalingRect, scalingRect, scalingRect);
		} else if (point.first == Item::bottomRightX) {
			painter->drawRect(x2 - scalingRect, y2, scalingRect, scalingRect);
		} else if (point.first == Item::topLeftY) {
			painter->drawRect(x1 - scalingRect, y1, scalingRect, scalingRect);
		} else if (point.first == Item::bottomRightY) {
			painter->drawRect(x2, y2 - scalingRect, scalingRect, scalingRect);
		}
	}
}

void Item::alignToGrid()
{
    setX(alignedCoordToGrid(pos().x()));
    setY(alignedCoordToGrid(pos().y()));

    setX1(alignedCoordToGrid(x1()));
    setY1(alignedCoordToGrid(y1()));
    setX2(alignedCoordToGrid(x2()));
    setY2(alignedCoordToGrid(y2()));
}

qreal Item::alignedCoordToGrid(qreal sceneCoord)
{
    if (SettingsManager::value("ActivateGrid").toBool()) {
        const int indexGrid = SettingsManager::value("IndexGrid").toInt();
        int coef = static_cast<int>(sceneCoord) / indexGrid;
        return alignedCoordinate(sceneCoord, coef, indexGrid);
    }
    return sceneCoord;
}

qreal Item::alignedCoordinate(qreal coord, int coef, int indexGrid)
{
    const int coefSign = coef != 0 ? coef / qAbs(coef) : 0;

    if (qAbs(qAbs(coord) - qAbs(coef) * indexGrid) <= indexGrid / 2) {
        return coef * indexGrid;
    } else if (qAbs(qAbs(coord) - (qAbs(coef) + 1) * indexGrid) <= indexGrid / 2) {
        return (coef + coefSign) * indexGrid;
    }
    return coord;
}

CreateItemPushButton *Item::createButton()
{
    CreateItemPushButton *button = new CreateItemPushButton(this);
    customizeButton(button);
    button->setMaximumSize(QSize(65, 16777215));
    button->setCheckable(true);
    button->setChecked(false);
    button->setAutoExclusive(false);
    button->setAutoDefault(false);
    button->setDefault(false);
    return button;
}

void Item::customizeButton(CreateItemPushButton *button) const
{
    button->setObjectName("itemPushButton");
}

void Item::calcForChangeScalingState(const QPointF&pos, const QPointF& point1, const QPointF& point2
		, int correction)
{
	qreal x = pos.x();
	qreal y = pos.y();
	qreal x1= point1.x();
	qreal x2 = point2.x();
	qreal y1 = point1.y();
	qreal y2 = point2.y();

	if (QRectF(x1 + scenePos().x() - correction, y1 - scalingRect + scenePos().y() - correction
			, scalingRect, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = topLeftX;
	} else if (QRectF(x2 - scalingRect + scenePos().x() + correction, y1 - scalingRect + scenePos().y() - correction
			, scalingRect, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = topRightX;
	} else if (QRectF(x1 + scenePos().x() - correction, y2 + scenePos().y(), scalingRect + correction
			, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = bottomLeftX;
	} else if (QRectF(x2 - scalingRect + scenePos().x() + correction, y2 + scenePos().y() + correction
			, scalingRect, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = bottomRightX;
	} else if (QRectF(x1 - scalingRect + scenePos().x() - correction, y1 + scenePos().y() - correction
			, scalingRect, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = topLeftY;
	} else if (QRectF(x2 + scenePos().x() + correction, y1 + scenePos().y() - correction
			, scalingRect, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = topRightY;
	} else if (QRectF(x1 - scalingRect + scenePos().x() - correction, y2 - scalingRect + scenePos().y() + correction
			, scalingRect, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = bottomLeftY;
	} else if (QRectF(x2 + scenePos().x() + correction, y2 - scalingRect + scenePos().y() + correction
			, scalingRect, scalingRect).contains(QPointF(x, y)))
	{
		mScalingState = bottomRightY;
	} else {
		mScalingState = noneScale;
	}
	if (mScalingState == topLeftX || mScalingState == topLeftY || mScalingState == bottomRightX
			|| mScalingState == bottomRightY)
	{
        setDragState(None);
	}
}

void Item::changeScalingPointState(qreal x, qreal y)
{
	QRectF itemBoundingRect = calcNecessaryBoundingRect();
	qreal x1= itemBoundingRect.left();
	qreal x2 = itemBoundingRect.right();
	qreal y1 = itemBoundingRect.top();
	qreal y2 = itemBoundingRect.bottom();
	int correction = 0;
	calcForChangeScalingState(QPointF(x, y), QPointF(x1, y1), QPointF(x2, y2), correction);
}

Item::ScalingPointState Item::getScalingPointState() const
{
	return mScalingState;
}

QColor Item::changeScaleColor(QPair<Item::ScalingPointState, QColor> point)
{
	if(point.second == QColor(Qt::black)) {
		return QColor(Qt::red);
	} else {
		return QColor(Qt::black);
	}
}

void Item::setScalingPointColor()
{
	for (int i = 0; i < mListScalePoint.size(); ++i) {
		QPair<Item::ScalingPointState, QColor> point = mListScalePoint.at(i);
		if (point.first == mScalingState) {
			mListScalePoint.insert(i, QPair<Item::ScalingPointState, QColor>(mScalingState, changeScaleColor(point)));
			mListScalePoint.removeAt(i + 1);
			return;
		}
	}
}

void Item::setListScalePoint(QList<QPair<ScalingPointState, QColor> > list)
{
	mListScalePoint = list;
}

QString Item::setScaleForDoc(int i, const QRect &rect)
{
	QString text;

	switch (i) {
	case 0:
		text = QString::number(rect.left());
		break;
	case 4:
		text = QString::number(rect.top());
		break;
	case 3:
		text = QString::number(rect.right());
		break;
	case 7:
		text = QString::number(rect.bottom());
		break;
	case 2:
		text = QString::number(rect.left());
		break;
	case 5:
		text = QString::number(rect.top());
		break;
	case 1:
		text = QString::number(rect.right());
		break;
	case 6:
		text = QString::number(rect.bottom());
		break;
	}

	if (mListScalePoint.at(i).second == QColor(Qt::red)) {
		text += "a";
	}

	return text;
}

QString Item::setSingleScaleForDoc(int i, int x, int y)
{
	QString text = "";

	if (i == 0) {
		text = QString::number(x);
	} else if (i == 4) {
		text = QString::number(y);
	}

	if (mListScalePoint.at(i).second == QColor(Qt::red)) {
		text += "a";
	}
	return text;
}

void Item::setXandY(QDomElement& dom, const QRectF &rect)
{
	dom.setAttribute("y1", setScaleForDoc(4, rect.toRect()));
	dom.setAttribute("x1", setScaleForDoc(0, rect.toRect()));
	dom.setAttribute("y2", setScaleForDoc(7, rect.toRect()));
	dom.setAttribute("x2", setScaleForDoc(3, rect.toRect()));
}

void Item::setVisibilityCondition(const VisibilityCondition &condition)
{
	mVisibilityCondition = condition;
}

void Item::setVisibilityCondition(const QString &property, const QString &sign, const QString &value)
{
	mVisibilityCondition.property = property;
	mVisibilityCondition.sign = sign;
	mVisibilityCondition.value = value;
}

VisibilityCondition Item::visibilityCondition() const
{
	return mVisibilityCondition;
}

QPair<QDomElement, Item::DomElementTypes> Item::generateDom(QDomDocument &document, const QPoint &topLeftPicture)
{
	QPair<QDomElement, Item::DomElementTypes> result = generateItem(document, topLeftPicture);

	if (!mVisibilityCondition.property.isEmpty() && !mVisibilityCondition.value.isEmpty()) {
		QDomElement visibility = document.createElement("showIf");
		result.first.appendChild(visibility);
		visibility.setAttribute("property", mVisibilityCondition.property);
		visibility.setAttribute("sign", mVisibilityCondition.sign);
		visibility.setAttribute("value", mVisibilityCondition.value);
	}

	return result;
}
