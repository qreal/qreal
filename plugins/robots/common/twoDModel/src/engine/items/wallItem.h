#pragma once

#include "lineItem.h"

namespace twoDModel {
namespace items {

class WallItem : public LineItem
{
	Q_OBJECT

public:
	WallItem(const QPointF &begin, const QPointF &end);

	AbstractItem* clone() const override;

	QPointF begin();
	QPointF end();
	bool isDragged() const;
	qreal width() const;

	/// Draws selection rect around sensorBoundingBox
	void drawExtractionForItem(QPainter *painter) override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) override;
	void deserializePenBrush(const QDomElement &element);

	void onOverlappedWithRobot(bool overlapped = true);

	QPainterPath path() const;

signals:
	void wallDragged(WallItem *item, const QPainterPath &shape, const QRectF &oldPos);

protected:
	void setPrivateData();

private:
	void recalculateBorders();

	bool mDragged;
	bool mOverlappedWithRobot;
	const QImage mImage;

	int mOldX1;
	int mOldY1;

	QPainterPath mPath;
};

}
}
