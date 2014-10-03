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
	SonarSensorItem(
			model::WorldModel const &worldModel
			, model::SensorsConfiguration &configuration
			, interpreterBase::robotModel::PortInfo const &port
			, QString const &pathToImage
			, QRect const &imageSize
			);

	virtual void drawItem(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget);
	virtual void drawExtractionForItem(QPainter *painter);
	virtual QRectF boundingRect() const;
	QPainterPath shape() const;

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

	/// Returns scanning region of a sensor as painter path, in relative to sensor coordinates.
	QPainterPath scanningRegion() const;

	model::WorldModel const &mWorldModel;
	QImage const mIcon;
};

}
}
