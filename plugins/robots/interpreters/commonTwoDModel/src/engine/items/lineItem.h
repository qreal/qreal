#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtXml/QDomDocument>

#include <qrutils/graphicsUtils/lineImpl.h>
#include "colorFieldItem.h"

namespace twoDModel {
namespace items {

class LineItem : public QObject, public ColorFieldItem
{
	Q_OBJECT

public:
	LineItem(QPointF const &begin, QPointF const &end, int cornerRadius = 0);

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

	QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture) override;
	void deserialize(QDomElement const &element) override;

	void deserializePenBrush(QDomElement const &element);
	void setSerializeName(QString const &name);

protected:
	void setPrivateData();
	qreal alignedCoordinate(qreal coord, int coef, int const indexGrid) const;
	void setBeginCoordinatesWithGrid(int indexGrid);
	void setEndCoordinatesWithGrid(int indexGrid);

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
