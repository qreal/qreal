#pragma once

#include <QtCore/QSet>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/pointImpl.h>
#include <qrutils/graphicsUtils/rotateItem.h>

#include "sensorsConfiguration.h"
#include "rotater.h"

namespace twoDModel {

/** @brief Class that represents robot port in 2D model */
class SensorItem : public QObject, public graphicsUtils::RotateItem
{
	Q_OBJECT
public:
//	SensorItem(SensorsConfiguration &configuration, robots::enums::inputPort::InputPortEnum port);

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

	void addStickyItem(QGraphicsItem *item);
	void removeStickyItem(QGraphicsItem *item);

	void onPositionChanged();
	void onDirectionChanged();

protected:
	QRectF imageRect() const;
	QString name() const;
	QString pathToImage() const;

	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	SensorsConfiguration &mConfiguration;
//	robots::enums::inputPort::InputPortEnum const mPort;
	QPointF mRotatePoint;
	bool mDragged;
	graphicsUtils::PointImpl mPointImpl;
	Rotater *mRotater;
	QSet<QGraphicsItem *> mStickyItems;

	QRectF const mImageRect;
	QRectF const mBoundingRect;
	QImage const mImage;
};

}
