#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "sensorsConfiguration.h"
#include "rotater.h"
#include "../../../../../qrutils/graphicsUtils/abstractItem.h"
#include "../../../../../qrutils/graphicsUtils/pointImpl.h"
#include "../../../../../qrutils/graphicsUtils/rotateInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/** @brief Class that represents robot port in 2D model */
class SensorItem : public QObject, public graphicsUtils::AbstractItem, public graphicsUtils::RotateInterface
{
	Q_OBJECT
public:
	SensorItem(SensorsConfiguration &configuration, inputPort::InputPortEnum port);

	virtual void rotate(double angle);
	virtual QRectF rect() const;
	virtual double rotateAngle() const;
	virtual void setSelected(bool isSelected);
	void setRotater(Rotater *rotater);
	virtual void checkSelection();

	/// Draws selection rect around sensorBoundingBox
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);

	QRectF boundingRect() const;

	virtual void changeDragState(qreal x, qreal y);
	virtual void resizeItem(QGraphicsSceneMouseEvent *event);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

	void setRotatePoint(QPointF rotatePoint);
	void setDeltaBasePosition(QPointF const &delta, qreal dir);
	void setBasePosition(QPointF const &pos, qreal dir);

protected:
	static int const size = 6;

	SensorsConfiguration &mConfiguration;
	inputPort::InputPortEnum const mPort;
	QPointF mBasePos;
	QPointF mRotatePoint;
	qreal mBaseDir;
	bool mDragged;
	graphicsUtils::PointImpl mPointImpl;
	Rotater *mRotater;

	QColor color() const;
	void setNewPosition(QPointF rotatePoint);
};

}
}
}
}
}
