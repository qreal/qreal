#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

#include "sensorItem.h"
#include "rotater.h"
#include "../../../../../qrutils/graphicsUtils/abstractItem.h"
#include "../../../../../qrutils/graphicsUtils/rectangleImpl.h"
#include "../../../../../qrutils/graphicsUtils/rotateInterface.h"
#include "robotModelInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const qreal robotWidth = 50;
const qreal robotHeight = 50;
const QPointF rotatePoint = QPointF(robotWidth / 2, robotHeight / 2);

/** @brief Class that represents a robot in 2D model */
class RobotItem : public QObject, public graphicsUtils::AbstractItem, public graphicsUtils::RotateInterface
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
	QPointF basePoint();

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

signals:
	void changedPosition();

private:
	/** @brief Image of a robot drawn on scene */
	QImage mImage;

	/** @brief List of sensors added to robot */
	QList<SensorItem *> mSensors;  // Does not have ownership

	/** @brief Previous position of robot (used while dragging to update sensors positions)*/
	qreal mPreviousAngle;
	QPointF mPreviousPos;

	QPointF mBasePoint;

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
