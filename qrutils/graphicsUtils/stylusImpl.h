#pragma once
#include <QtGui/QPainter>
#include "abstractItem.h"
#include "../utilsDeclSpec.h"

namespace graphicsUtils
{
class QRUTILS_EXPORT StylusImpl
{
public :
	StylusImpl();

	QRectF boundingRect(QList<AbstractItem *> listLine) const;
	QPainterPath shape(QList<AbstractItem *> listLine) const;

	void drawItem(QList<AbstractItem *> listLine, QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	void drawExtractionForItem(QList<AbstractItem *> listLine, QPainter* painter);
	void drawFieldForResizeItem(QPainter* painter);
	void drawScalingRects(QPainter* painter);

	void setPenStyle(QList<AbstractItem *> listLine, QString const &text);
	void setPenWidth(QList<AbstractItem *> listLine, int width);
	void setPenColor(QList<AbstractItem *> listLine, QString const &text);
	void setBrushStyle(QList<AbstractItem *> listLine, QString const &text);
	void setBrushColor(QList<AbstractItem *> listLine, QString const &text);

	void resizeItem(QGraphicsSceneMouseEvent *event);
private:
	QRectF searchMaxMinCoord(QList<AbstractItem *> listLine) const;
};
}
