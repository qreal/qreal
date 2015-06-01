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

#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPen>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include <QtCore/QPair>
#include <QtCore/QList>

#include <qrutils/graphicsUtils/abstractItem.h>

class Item : public graphicsUtils::AbstractItem
{
public:
	enum DomElementTypes {
		noneType
		, pictureType
		, labelType
		, portType
	};

	enum ScalingPointState {
		noneScale
		, topLeftX
		, topRightX
		, bottomLeftX
		, bottomRightX
		, topLeftY
		, topRightY
		, bottomLeftY
		, bottomRightY
	};

	struct VisibilityCondition
	{
		QString property;
		QString sign;
		QString value;

		bool operator==(const VisibilityCondition &other) const;
		bool operator!=(const VisibilityCondition &other) const;
	};

	Item(graphicsUtils::AbstractItem* parent = 0);
	virtual Item* clone() = 0;
	virtual void setItemZValue(int zValue);
	int itemZValue();
	static int sign(int x);
	static qreal length(const QPointF &point1, const QPointF &point2);
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) = 0;
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);

	static void swap(qreal &x, qreal &y);
	void setNoneDragState();

	void setListScalePoint(QList<QPair<ScalingPointState, QColor> > list);

	void initListScalePoint();
	void calcForChangeScalingState(const QPointF&pos, const QPointF& point1, const QPointF& point2, int correction);
	virtual void changeScalingPointState(qreal x, qreal y);
	ScalingPointState getScalingPointState() const;
	QColor changeScaleColor(QPair<Item::ScalingPointState, QColor> point);
	void setScalingPointColor();

	//for save to xml
	QString setScaleForDoc(int i, const QRect &rect);
	QString setSingleScaleForDoc(int i, int x, int y);
	virtual void setXandY(QDomElement& dom, const QRectF &rect);

	QPair<QDomElement, Item::DomElementTypes> generateDom(QDomDocument &document, const QPoint &topLeftPicture);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture) = 0;

	void setVisibilityCondition(const VisibilityCondition &condition);
	void setVisibilityCondition(const QString &property, const QString &sign, const QString &value);
	VisibilityCondition visibilityCondition() const;

protected:
	QList<QPair<ScalingPointState, QColor> > mListScalePoint;
	bool mNeedScalingRect;
	DomElementTypes mDomElementType;
	ScalingPointState mScalingState;
	int mZValue;
	VisibilityCondition mVisibilityCondition;
};
