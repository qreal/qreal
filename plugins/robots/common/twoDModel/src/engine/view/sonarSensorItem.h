#pragma once

#include <kitBase/robotModel/portInfo.h>

#include "sensorItem.h"
#include "src/engine/model/worldModel.h"
#include "src/engine/model/sensorsConfiguration.h"

namespace twoDModel {
namespace view {

/// Sonar sensor that can draw its scanning area.
class SonarSensorItem : public SensorItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	SonarSensorItem(const model::WorldModel &worldModel
			, model::SensorsConfiguration &configuration
			, const kitBase::robotModel::PortInfo &port
			, const QString &pathToImage
			, const QRect &imageSize
			);

	void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget) override;
	void drawExtractionForItem(QPainter *painter) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	QRectF rect() const override;

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

	/// Returns scanning region of a sensor as painter path, in relative to sensor coordinates.
	QPainterPath scanningRegion() const;

	const model::WorldModel &mWorldModel;
	const QImage mIcon;
};

}
}
