#include "worldModel.h"

#include <QtGui/QTransform>
#include <QtCore/QStringList>

#include "../tracer.h"
#include "stylusItem.h"

using namespace qReal::interpreters::robots::details::d2Model;

WorldModel::WorldModel()
{
}

int WorldModel::sonarReading(QPoint const &position, qreal direction) const
{
	int const maxSonarRangeCms = 255;

	QPainterPath const wallPath = buildWallPath();

	for (int currentRangeInCm = 1; currentRangeInCm <= maxSonarRangeCms; ++currentRangeInCm) {
		QPainterPath rayPath = sonarScanningRegion(position, direction, currentRangeInCm);
		if (rayPath.intersects(wallPath)) {
			Tracer::debug(tracer::d2Model, "WorldModel::sonarReading", "Sonar sensor. Reading: " + QString(currentRangeInCm));
			return currentRangeInCm;
		}
	}

	Tracer::debug(tracer::d2Model, "WorldModel::sonarReading", "Sonar sensor. Reading: max (" + QString(maxSonarRangeCms) + ")");
	return maxSonarRangeCms;
}

bool WorldModel::touchSensorReading(QPoint const &position, qreal direction, inputPort::InputPortEnum const port)
{
	Q_UNUSED(direction)

	QPainterPathStroker pathStroker;
	pathStroker.setWidth(3);
	QPainterPath robotPath;

	robotPath.moveTo(mTouchSensorPositionOld[port]);
	robotPath.lineTo(position);

	mTouchSensorPositionOld[port] = position;

	QPainterPath wallPath = buildWallPath();
	return wallPath.intersects(robotPath);
}

QPainterPath WorldModel::sonarScanningRegion(QPoint const &position, qreal direction, int range) const
{
	double const pixelsInCm = 1;
	double const rayWidthDegrees = 10.0;
	double const rangeInPixels = range * pixelsInCm;

	QPainterPath rayPath;
	rayPath.arcTo(QRect(-rangeInPixels, -rangeInPixels
			, 2 * rangeInPixels, 2 * rangeInPixels)
			, -rayWidthDegrees, 2 * rayWidthDegrees);
	rayPath.closeSubpath();
	QTransform sensorPositionTransform = QTransform().rotate(direction)
			.translate(position.x(), position.y());
	return sensorPositionTransform.map(rayPath);
}

bool WorldModel::checkCollision(QPolygonF const &robotRegion) const
{
	QPainterPath robotPath;
	robotPath.addPolygon(robotRegion);
	QPainterPathStroker pathStroker;
	pathStroker.setWidth(3);
	robotPath = pathStroker.createStroke(robotPath);

	QPainterPath wallPath = buildWallPath();
	return wallPath.intersects(robotPath);
}

QList<WallItem *> const &WorldModel::walls() const
{
		return mWalls;
}

void WorldModel::addWall(WallItem* wall)
{
	mWalls.append(wall);
}

QList<ColorFieldItem *> const &WorldModel::colorFields() const
{
		return mColorFields;
}

void WorldModel::addColorField(ColorFieldItem* colorField)
{
		mColorFields.append(colorField);
}

void WorldModel::clearScene()
{
	mWalls.clear();
		mColorFields.clear();
}

QPainterPath WorldModel::buildWallPath() const
{
	// TODO: Maintain a cache for this.
	QPainterPathStroker pathStroker;
	pathStroker.setWidth(3);
	QPainterPath wallPath;

	foreach (WallItem* wall, mWalls) {
		wallPath.moveTo(wall->begin());
		wallPath.lineTo(wall->end());
	}
	return wallPath;
}

QDomElement WorldModel::serialize(QDomDocument &document, QPoint const &topLeftPicture) const
{
	QDomElement result = document.createElement("world");

	QDomElement walls = document.createElement("walls");
	result.appendChild(walls);
	foreach (WallItem *wall, mWalls) {
				QDomElement wallNode = wall->serialize(document, topLeftPicture);
		walls.appendChild(wallNode);
	}

		QDomElement colorFields = document.createElement("colorFields");
		result.appendChild(colorFields);
		foreach (ColorFieldItem *colorField, mColorFields) {
				QDomElement colorFiedlNode = colorField->serialize(document, topLeftPicture);
				colorFields.appendChild(colorFiedlNode);
		}

	return result;
}

void WorldModel::deserialize(QDomElement const &element)
{
	if (element.isNull()) {
		// TODO: Report error
		return;
	}

	QDomNodeList walls = element.elementsByTagName("wall");
	for (int i = 0; i < walls.count(); ++i) {
		QDomElement const wallNode = walls.at(i).toElement();

		WallItem *wall = new WallItem(QPointF(0, 0), QPointF(0, 0));
		wall->deserialize(wallNode);
		mWalls.append(wall);
	}

		QDomNodeList colorFields = element.elementsByTagName("colorFields");
		for (int i = 0; i < colorFields.count(); ++i) {
				QDomElement const colorFieldNode = colorFields.at(i).toElement();

				QDomNodeList lines = colorFieldNode.elementsByTagName("line");
				for (int i = 0; i < lines.count(); ++i) {
						QDomElement const lineNode = lines.at(i).toElement();
						LineItem* lineItem = new LineItem(QPointF(0, 0), QPointF(0, 0));
						lineItem->deserialize(lineNode);
						mColorFields.append(lineItem);
				}

				QDomNodeList styluses = colorFieldNode.elementsByTagName("stylus");
				for (int i = 0; i < styluses.count(); ++i) {
						QDomElement const stylusNode = styluses.at(i).toElement();
						StylusItem *stylusItem = new StylusItem(0, 0);
						stylusItem->deserialize(stylusNode);
						mColorFields.append(stylusItem);
				}
		}
}
