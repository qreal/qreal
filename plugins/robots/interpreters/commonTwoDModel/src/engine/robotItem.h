#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/rectangleImpl.h>
#include <qrutils/graphicsUtils/rotateItem.h>

#include "sensorItem.h"
#include "rotater.h"
#include "robotModelInterface.h"

namespace twoDModel {

class BeepItem : public QGraphicsItem
{
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;

private:
	void drawBeep(QPainter *painter);
	void drawBeepArcs(QPainter *painter, QPointF const &center, qreal radius);
};

/** @brief Class that represents a robot in 2D model */
class RobotItem : public QObject, public graphicsUtils::RotateItem
{
	Q_OBJECT
public:
	RobotItem();

	virtual void rotate(qreal angle);
	virtual QRectF rect() const;
	virtual qreal rotateAngle() const;
	void setRotateAngle(double const &angle);
	virtual void setSelected(bool isSelected);
	void setRotater(Rotater *rotater);
	virtual void checkSelection();
	void setRobotPos(QPointF const &newPos);
	QPointF robotPos(void);

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

	void recoverDragStartPosition();

	void processPositionChange();
	void processPositionAndAngleChange();

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
	void mousePressed();
	void changedPosition();

private:
	void onLanded();

	/** @brief Image of a robot drawn on scene */
	QImage mImage;
	// Takes ownership
	BeepItem *mBeepItem;

	/** @brief List of sensors added to robot */
	QList<SensorItem *> mSensors;  // Does not have ownership

	bool mIsOnTheGround;
	QPointF mDragStart;
	Rotater *mRotater;
	graphicsUtils::RectangleImpl mRectangleImpl;

	RobotModelInterface *mRobotModel;
};

}
