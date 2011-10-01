#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>
#include "../../../../../qrutils/graphicsUtils/abstractItem.h"
#include "../../../../../qrutils/graphicsUtils/lineImpl.h"
#include "../../../../../qrutils/graphicsUtils/rotateInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const int length = 30;
const int addAngle = 30; //в градусах; для отрисовки наконечника стрелки

/// Class that represents line that allows to rotate QGraphicItems
class Rotater : public QObject, public graphicsUtils::AbstractItem
{
	Q_OBJECT
public:
	Rotater();

	virtual QRectF boundingRect() const;

	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void drawFieldForResizeItem(QPainter* painter);
	virtual void setPenBrushForExtraxtion(QPainter* painter, const QStyleOptionGraphicsItem* option);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);
	virtual void calcResizeItem(QGraphicsSceneMouseEvent *event);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

	void setMasterItem(graphicsUtils::RotateInterface *masterItem);
	void reshapeWithMasterItem(QPointF pos);
	void rotateWithMasterItem(QPointF delta, QPointF rotatePoint, QPointF basePos, qreal baseDir, qreal localDir);


private:
	int mLength;
	qreal mMasterSize;
	QPointF mMasterRotatePoint;
	QPointF mFrom;
	QPointF mTo;
	graphicsUtils::RotateInterface *mMaster;
	QGraphicsLineItem mLine;
	graphicsUtils::LineImpl mLineImpl;
};

}
}
}
}
}
