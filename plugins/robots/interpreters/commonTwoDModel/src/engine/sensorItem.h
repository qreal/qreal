#pragma once

#include <QtCore/QSet>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/pointImpl.h>
#include <qrutils/graphicsUtils/rotateItem.h>

#include <interpreterBase/robotModel/portInfo.h>

#include "sensorsConfiguration.h"
#include "rotater.h"

namespace twoDModel {

/// Class that represents sensor in 2D model.
class SensorItem : public QObject, public graphicsUtils::RotateItem
{
	Q_OBJECT

public:
	class PortItem : public QGraphicsItem
	{
	public:
		explicit PortItem(interpreterBase::robotModel::PortInfo const &port);

	protected:
		void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget) override;
		QRectF boundingRect() const override;

	private:
		interpreterBase::robotModel::PortInfo const mPort;
	};

	SensorItem(SensorsConfiguration &configuration, interpreterBase::robotModel::PortInfo const &port);

	void rotate(double angle) override;
	QRectF rect() const override;
	double rotateAngle() const override;
	void setSelected(bool isSelected) override;
	void setRotater(Rotater *rotater);
	void checkSelection() override;

	/// Draws selection rect around sensorBoundingBox
	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;

	QRectF boundingRect() const override;

	void changeDragState(qreal x, qreal y) override;
	void resizeItem(QGraphicsSceneMouseEvent *event) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

	void setRotatePoint(QPointF const &rotatePoint);

	void addStickyItem(QGraphicsItem *item);
	void removeStickyItem(QGraphicsItem *item);

	void onPositionChanged();
	void onDirectionChanged();

protected:
	QRectF imageRect() const;
	QString name() const;
	QString pathToImage() const;

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	SensorsConfiguration &mConfiguration;
	interpreterBase::robotModel::PortInfo const mPort;
	QPointF mRotatePoint;
	bool mDragged;
	graphicsUtils::PointImpl mPointImpl;
	Rotater *mRotater;
	QSet<QGraphicsItem *> mStickyItems;

	QRectF const mImageRect;
	QRectF const mBoundingRect;
	QImage const mImage;
	PortItem *mPortItem;  // Has ownership.
};

}
