#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>

#include <qrutils/graphicsUtils/lineImpl.h>
#include "colorFieldItem.h"

namespace twoDModel {

class LineItem : public QObject, public ColorFieldItem
{
	Q_OBJECT

public:
	LineItem(QPointF const &begin, QPointF const &end, int cornerRadius = 0);
	QPainterPath shape() const;
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);

	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
	virtual void reshapeRectWithShift();
	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event, int indexGrid);

	virtual void resizeBeginWithGrid(int indexGrid);
	virtual void reshapeEndWithGrid(int indexGrid);
	virtual void reshapeBeginWithGrid(int indexGrid);
	virtual void setBeginCoordinatesWithGrid(int indexGrid);
	virtual void setEndCoordinatesWithGrid(int indexGrid);
	virtual void setDraggedEndWithGrid(qreal x, qreal y);

	virtual QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture);
	virtual void deserialize(QDomElement const &element);
	virtual void deserializePenBrush(QDomElement const &element);
	void setSerializeName(QString name);

protected:
	void setPrivateData();
	qreal alignedCoordinate(qreal coord, int coef, int const indexGrid) const;

	graphicsUtils::LineImpl mLineImpl;
	QString mSerializeName;
	int mCornerRadius;
	int mCellNumbX1;
	int mCellNumbY1;
	int mCellNumbX2;
	int mCellNumbY2;
};

}
