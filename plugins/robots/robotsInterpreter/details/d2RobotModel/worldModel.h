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

int const sensorWidth = 10;
qreal const pixelsInCm = 50 / 14; // robot item height in px / robot real height

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class WorldModel
{
public:
	WorldModel();
	int sonarReading(QPoint const &position, qreal direction) const;
	bool touchSensorReading(QPoint const &position, qreal direction, inputPort::InputPortEnum const port);
	QPainterPath sonarScanningRegion(QPoint const &position, qreal direction, int range = 255) const;
	QPainterPath sonarScanningRegion(QPoint const &position, int range = 255) const;
	bool checkCollision(QPolygonF const &robotRegion) const;
	QList<WallItem *> const &walls() const;
	QList<ColorFieldItem *> const &colorFields() const;

	void addWall(WallItem* wall);
	void removeWall(WallItem* wall);
	void addColorField(ColorFieldItem* colorField);
	void removeColorField(ColorFieldItem* colorField);
	void clearScene();

	QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture) const;
	void deserialize(QDomElement const &element);

private:
	bool checkSonarDistance(int const distance, QPoint const &position
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
