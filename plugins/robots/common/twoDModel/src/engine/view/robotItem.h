#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/rectangleImpl.h>
#include <qrutils/graphicsUtils/rotateItem.h>

#include "sensorItem.h"
#include "rotater.h"

#include "src/engine/model/robotModel.h"

namespace twoDModel {
namespace view {

/** @brief Class that represents a robot in 2D model */
class RobotItem : public graphicsUtils::RotateItem
{
	Q_OBJECT

public:
	RobotItem(const QString &robotImageFileName, model::RobotModel &robotModel);

	QRectF rect() const override;
	void setSelected(bool isSelected) override;
	void checkSelection() override;

	/// Returns robot`s rotater item.
	Rotater *rotater() const;
	void setRotater(Rotater *rotater);

	QRectF boundingRect() const override;
	QRectF calcNecessaryBoundingRect() const override;
	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	void drawExtractionForItem(QPainter *painter) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void resizeItem(QGraphicsSceneMouseEvent *event) override;

	/// Returns a mapping of ports to connected configured sensors.
	QMap<kitBase::robotModel::PortInfo, SensorItem *> const &sensors() const;

	void addSensor(const kitBase::robotModel::PortInfo &port, SensorItem *sensor);
	void removeSensor(const kitBase::robotModel::PortInfo &port);
	void updateSensorPosition(const kitBase::robotModel::PortInfo &port);
	void updateSensorRotation(const kitBase::robotModel::PortInfo &port);

	void setNeededBeep(bool isNeededBeep);

	void recoverDragStartPosition();
	model::RobotModel &robotModel();

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

signals:
	void mousePressed();
	void changedPosition(RobotItem *robotItem);
	void drawTrace(const QPen &pen, const QPointF &from, const QPointF &to);

private:
	class BeepItem : public QGraphicsItem
	{
	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
		QRectF boundingRect() const;

	private:
		void drawBeep(QPainter *painter);
		void drawBeepArcs(QPainter *painter, const QPointF &center, qreal radius);
	};

	/// Same as QGraphicsItem::setPos(). Needed as slot for connection.
	void setPos(const QPointF &newPos);
	/// Same as QGraphicsItem::setRotation(). Needed as slot for connection.
	void setRotation(qreal rotation) override;
	void ride(const QPointF &newPos, qreal rotation);

	void onLanded();

	/** @brief Image of a robot drawn on scene */
	QImage mImage;
	// Takes ownership
	BeepItem *mBeepItem;

	QMap<kitBase::robotModel::PortInfo, SensorItem *> mSensors;  // Does not have ownership

	QPointF mDragStart;
	Rotater *mRotater;
	graphicsUtils::RectangleImpl mRectangleImpl;
	QPointF mMarkerPoint;

	model::RobotModel &mRobotModel;
};

}
}
