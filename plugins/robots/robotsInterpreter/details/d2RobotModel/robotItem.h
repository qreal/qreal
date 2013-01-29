#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "sensorItem.h"
#include "rotater.h"
#include "../../../../../qrutils/graphicsUtils/abstractItem.h"
#include "../../../../../qrutils/graphicsUtils/rectangleImpl.h"
#include "../../../../../qrutils/graphicsUtils/rotateItem.h"
#include "robotModelInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const qreal robotWidth = 50;
const qreal robotHeight = 50;
const QPointF rotatePoint = QPointF(robotWidth / 2, robotHeight / 2);

const qreal beepWavesSize = 120;

class BeepItem : public QGraphicsItem
{
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect () const;

private:
	void drawBeep(QPainter* painter);
	void drawBeepArcs(QPainter* painter, QPointF const &center, qreal radius);

};

/** @brief Class that represents a robot in 2D model */
class RobotItem : public QObject, public graphicsUtils::RotateItem
{
	Q_OBJECT
public:
	RobotItem();

	virtual void rotate(qreal angle);
	virtual QRectF rect() const;
	virtual double rotateAngle() const;
	virtual void setSelected(bool isSelected);
	void setRotater(Rotater *rotater);
	virtual void checkSelection();

	virtual QRectF boundingRect() const;
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawExtractionForItem(QPainter* painter);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	void resizeItem(QGraphicsSceneMouseEvent *event);
	void setPos(QPointF const &newPos);

	/** @brief Add new sensor to mSensors */
	void addSensor(SensorItem *sensor);
	void removeSensor(SensorItem *sensor);

	/** @brief Clear mSensors */
	void clearSensors();

	/** @brief Returns false if we're dragging robot item somewhere */
	bool isOnTheGround() const;

	void setRobotModel(RobotModelInterface *robotModel);
	void setNeededBeep(bool isNeededBeep);

	void addSensorsShapes(QPainterPath &target);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
	void changedPosition();

private:
	/** @brief Image of a robot drawn on scene */
	QImage mImage;
	QImage mBeepImage;
	BeepItem *mBeepItem;

	/** @brief List of sensors added to robot */
	QList<SensorItem *> mSensors;  // Does not have ownership

	bool mIsOnTheGround;
	Rotater *mRotater;
	graphicsUtils::RectangleImpl mRectangleImpl;

	RobotModelInterface *mRobotModel;
};

}
}
}
}
}
