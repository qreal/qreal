#pragma once

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtGui/QPolygon>
#include <QtGui/QPainterPath>
#include <QtXml/QDomDocument>

#include "../../sensorConstants.h"
#include "wallItem.h"
#include "colorFieldItem.h"

qreal const pixelsInCm = 16 / 5.6; // robot`s wheel length in px / its real length

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class WorldModel
{
public:
	WorldModel();
	int sonarReading(QPointF const &position, qreal direction) const;
	bool touchSensorReading(QPointF const &position, qreal direction, inputPort::InputPortEnum const port);
	QPainterPath sonarScanningRegion(QPointF const &position, qreal direction, int range = 255) const;
	QPainterPath sonarScanningRegion(QPointF const &position, int range = 255) const;
	bool checkCollision(QPainterPath const &robotPath, int stroke = 3) const;
	QList<WallItem *> const &walls() const;
	QList<ColorFieldItem *> const &colorFields() const;

	void addWall(WallItem* wall);
	void removeWall(WallItem* wall);
	void addColorField(ColorFieldItem* colorField);
	void removeColorField(ColorFieldItem* colorField);
	void clearScene();

	QDomElement serialize(QDomDocument &document, QPointF const &topLeftPicture) const;
	void deserialize(QDomElement const &element);

private:
	bool checkSonarDistance(int const distance, QPointF const &position
			, qreal const direction, QPainterPath const &wallPath) const;

	QList<WallItem *> mWalls;
	QList<ColorFieldItem *> mColorFields;
	QMap<inputPort::InputPortEnum, QPointF> mTouchSensorPositionOld;
	QMap<inputPort::InputPortEnum, qreal> mTouchSensorDirectionOld;

	QPainterPath buildWallPath() const;
};

}
}
}
}
}
