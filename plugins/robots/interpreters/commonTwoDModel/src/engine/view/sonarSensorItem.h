#pragma once

#include <interpreterBase/robotModel/portInfo.h>

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
			, const interpreterBase::robotModel::PortInfo &port
			, const QString &pathToImage
			, const QRect &imageSize
			);

	virtual void drawItem(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);
	virtual void drawExtractionForItem(QPainter *painter);
	virtual QRectF boundingRect() const;
	QPainterPath shape() const;

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

	/// Returns scanning region of a sensor as painter path, in relative to sensor coordinates.
	QPainterPath scanningRegion() const;

	const model::WorldModel &mWorldModel;
	const QImage mIcon;
};

}
}
