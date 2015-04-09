#pragma once

#include <qrutils/graphicsUtils/lineImpl.h>

#include "colorFieldItem.h"

namespace twoDModel {
namespace items {

class LineItem : public ColorFieldItem
{
	Q_OBJECT

public:
	LineItem(const QPointF &begin, const QPointF &end, int cornerRadius = 0);

	AbstractItem *clone() const override;

	QPainterPath shape() const override;

	QRectF boundingRect() const override;
	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;

	void resizeItem(QGraphicsSceneMouseEvent *event) override;
	void reshapeRectWithShift() override;
	void calcResizeItem(QGraphicsSceneMouseEvent *event, int indexGrid);

	void resizeBeginWithGrid(int indexGrid);
	void reshapeEndWithGrid(int indexGrid);
	void reshapeBeginWithGrid(int indexGrid);
	void setDraggedEnd(qreal x, qreal y);
	void alignTheWall(int indexGrid);

	QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) override;
	void deserialize(const QDomElement &element) override;

	void deserializePenBrush(const QDomElement &element);
	void setSerializeName(const QString &name);

protected:
	void setPrivateData();
	qreal alignedCoordinate(qreal coord, int coef, const int indexGrid) const;
	void setBeginCoordinatesWithGrid(int indexGrid);
	void setEndCoordinatesWithGrid(int indexGrid);
	void countCellNumbCoordinates(int indexGrid);

	graphicsUtils::LineImpl mLineImpl;
	QString mSerializeName;
	int mCornerRadius;
	int mCellNumbX1;
	int mCellNumbY1;
	int mCellNumbX2;
	int mCellNumbY2;
};

}
}
