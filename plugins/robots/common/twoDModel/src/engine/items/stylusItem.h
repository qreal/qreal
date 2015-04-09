#pragma once

#include <qrutils/graphicsUtils/stylusImpl.h>

#include "lineItem.h"

namespace twoDModel {
namespace items {

class StylusItem : public ColorFieldItem
{
	Q_OBJECT

public:
	StylusItem(qreal x1, qreal y1);

	AbstractItem *clone() const override;

	void addLine(qreal x2, qreal y2);

	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;
	void drawFieldForResizeItem(QPainter* painter) override;
	void drawScalingRects(QPainter* painter);

	void setPenStyle(const QString &text) override;
	void setPenWidth(int width) override;
	void setPenColor(const QString &text) override;
	void setBrushStyle(const QString &text) override;
	void setBrushColor(const QString &text) override;

	void resizeItem(QGraphicsSceneMouseEvent *event) override;
	QDomElement serialize(QDomDocument &document, const QPoint &topLeftPicture) override;
	void deserialize(const QDomElement &element) override;

signals:
	/// Emitted when user draws new segment in this stylus line.
	void segmentAdded(LineItem *segment);

private:
	void recalculateProperties();

	qreal mTmpX1;
	qreal mTmpY1;
	QList<AbstractItem *> mAbstractListLine;
	graphicsUtils::StylusImpl mStylusImpl;
	QRectF mBoundingRect;
};

}
}
