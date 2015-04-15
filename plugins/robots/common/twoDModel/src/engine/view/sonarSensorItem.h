/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <kitBase/robotModel/portInfo.h>

#include "sensorItem.h"
#include "twoDModel/engine/model/worldModel.h"
#include "twoDModel/engine/model/sensorsConfiguration.h"

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
