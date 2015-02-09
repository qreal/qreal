#pragma once

#include <qrutils/graphicsUtils/pointImpl.h>

#include "mainWindow/shapeEdit/item.h"
#include "mainWindow/shapeEdit/ellipse.h"

class PointPort : public Item
{
public:
	PointPort(qreal x, qreal y, Item *parent = 0);
	PointPort(const PointPort &other);
	virtual Item* clone();
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void drawScalingRects(QPainter* painter);
	virtual void changeDragState(qreal x, qreal y);
	virtual void changeScalingPointState(qreal x, qreal y);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);

	virtual QPair<QDomElement, Item::DomElementTypes> generateItem(QDomDocument &document
			, const QPoint &topLeftPicture);

	void setType(const QString &type);
	QString getType() const;

private:
	qreal mRadius;
	qreal mUnrealRadius;
	graphicsUtils::PointImpl mPointImpl;
	QString mType;
};
