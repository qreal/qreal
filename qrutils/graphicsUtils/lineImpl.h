#pragma once

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QPainter>
#include "../utilsDeclSpec.h"

namespace graphicsUtils
{

class QRUTILS_EXPORT LineImpl
{
public:
	LineImpl();
	QLineF line(qreal x1, qreal y1, qreal x2, qreal y2) const;
	QPainterPath shape(const int drift, qreal x1, qreal y1, qreal x2, qreal y2) const;
	QRectF boundingRect(qreal x1, qreal y1, qreal x2, qreal y2, qreal penWidth, const int drift) const;
	QRectF realBoundingRectWithoutScene(qreal x1, qreal y1, qreal x2, qreal y2, qreal penWidth, const int drift) const;
	void drawItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawPointExtractionForItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2);
	void drawExtractionForItem(QPainter* painter, qreal x1, qreal y1, qreal x2, qreal y2, const int drift);
	void drawFieldForResizeItem(QPainter* painter, const int resizeDrift, qreal x1, qreal y1, qreal x2, qreal y2);
	QPair<qreal, qreal> reshapeRectWithShiftForLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal differenceX, qreal differenceY, qreal size);
};
}
