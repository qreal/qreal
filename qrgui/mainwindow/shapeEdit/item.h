#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPen>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>
#include <QtCore/QPair>
#include <QtCore/QList>
#include "../../../qrutils/graphicsUtils/abstractItem.h"

class Item : public graphicsUtils::AbstractItem
{
public:
	enum DomElementTypes {
		noneType,
		pictureType,
		labelType,
		portType
	};

	enum ScalingPointState {
		noneScale,
		topLeftX,
		topRightX,
		bottomLeftX,
		bottomRightX,
		topLeftY,
		topRightY,
		bottomLeftY,
		bottomRightY
	};

	Item(graphicsUtils::AbstractItem* parent = 0);
	virtual Item* clone() = 0;
	virtual void setItemZValue(int zValue);
	int itemZValue();
	static int sign(int x);
	static qreal length(QPointF const &point1, QPointF const &point2);
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) = 0;
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);

	static void swap(qreal &x, qreal &y);
	void setNoneDragState();

	void setListScalePoint(QList<QPair<ScalingPointState, QColor> > list);

	void initListScalePoint();
	void calcForChangeScalingState(QPointF const&pos, QPointF const& point1, QPointF const& point2, int const &correction);
	virtual void changeScalingPointState(qreal x, qreal y);
	ScalingPointState getScalingPointState() const;
	QColor changeScaleColor(QPair<Item::ScalingPointState, QColor> point);
	void setScalingPointColor();

	//for save to xml
	QString setScaleForDoc(int i, QRect const &rect);
	QString setSingleScaleForDoc(int i, int x, int y);
	virtual void setXandY(QDomElement& dom, QRectF const &rect);
	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document, QPoint const &topLeftPicture) = 0;

protected:
	QList<QPair<ScalingPointState, QColor> > mListScalePoint;
	bool mNeedScalingRect;
	DomElementTypes mDomElementType;
	ScalingPointState mScalingState;
	int mZValue;
};
