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

#include <QtCore/QSet>
#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>

#include <qrutils/graphicsUtils/abstractItem.h>
#include <qrutils/graphicsUtils/pointImpl.h>
#include <qrutils/graphicsUtils/rotateItem.h>

#include <kitBase/robotModel/portInfo.h>

#include "twoDModel/engine/model/sensorsConfiguration.h"

namespace twoDModel {
namespace view {

/// Class that represents sensor in 2D model.
class SensorItem : public graphicsUtils::RotateItem
{
	Q_OBJECT

public:
	SensorItem(model::SensorsConfiguration &configuration
			, const kitBase::robotModel::PortInfo &port, const QString &pathToImage, const QRect &imageSize);

	/// Draws selection rect around sensorBoundingBox
	void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
	void drawExtractionForItem(QPainter* painter) override;

	QRectF boundingRect() const override;

	void changeDragState(qreal x, qreal y) override;
	void resizeItem(QGraphicsSceneMouseEvent *event) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

protected:
	class PortItem : public QGraphicsItem
	{
	public:
		explicit PortItem(const kitBase::robotModel::PortInfo &port);

		QRectF boundingRect() const override;

	protected:
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	private:
		const kitBase::robotModel::PortInfo mPort;
		const QFont mFont;
		const QRect mBoundingRect;
	};

	QRectF imageRect() const;
	QString name() const;
	QString pathToImage() const;

	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	model::SensorsConfiguration &mConfiguration;
	const kitBase::robotModel::PortInfo mPort;
	graphicsUtils::PointImpl mPointImpl;

	const QRectF mImageRect;
	const QRectF mBoundingRect;
	const QImage mImage;
	PortItem *mPortItem;  // Has ownership.
};

}
}
