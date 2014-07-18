#include <QtGui/QTransform>
#include <QtCore/QStringList>

#include "constants.h"
#include "worldModel.h"

#include "src/engine/items/wallItem.h"
#include "src/engine/items/colorFieldItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/stylusItem.h"

using namespace twoDModel;
using namespace model;

WorldModel::WorldModel()
{
}

/// Measure the distance between robot and wall
int WorldModel::sonarReading(QPointF const &position, qreal direction) const
{
	int maxSonarRangeCms = 255;
	int minSonarRangeCms = 0;
	int currentRangeInCm = (minSonarRangeCms + maxSonarRangeCms) / 2;

	QPainterPath const wallPath = buildWallPath();
	if (!checkSonarDistance(maxSonarRangeCms, position, direction, wallPath)) {
		return maxSonarRangeCms;
	}

	for ( ; minSonarRangeCms < maxSonarRangeCms;
			currentRangeInCm = (minSonarRangeCms + maxSonarRangeCms) / 2)
	{
		if (checkSonarDistance(currentRangeInCm, position, direction, wallPath)) {
			maxSonarRangeCms = currentRangeInCm;
		} else {
			minSonarRangeCms = currentRangeInCm + 1;
		}
	}

	return currentRangeInCm;
}

/// Check intersection ray with wall
bool WorldModel::checkSonarDistance(int const distance, QPointF const &position
		, qreal const direction, QPainterPath const &wallPath) const
{
	QPainterPath const rayPath = sonarScanningRegion(position, direction, distance);
	return rayPath.intersects(wallPath);
}

/// Draw area, which is seen by sensor from 0
QPainterPath WorldModel::sonarScanningRegion(QPointF const &position, int range) const
{
	return sonarScanningRegion(position, 0, range);
}

/// Draw area, which is seen by sensor
QPainterPath WorldModel::sonarScanningRegion(QPointF const &position, qreal direction, int range) const
{
	qreal const rayWidthDegrees = 10.0;
	qreal const rangeInPixels = range * pixelsInCm;

	QPainterPath rayPath;
	rayPath.arcTo(QRect(-rangeInPixels, -rangeInPixels
			, 2 * rangeInPixels, 2 * rangeInPixels)
			, -direction - rayWidthDegrees, 2 * rayWidthDegrees);
	rayPath.closeSubpath();
	QTransform const sensorPositionTransform = QTransform().translate(position.x(), position.y());
	return sensorPositionTransform.map(rayPath);
}

/// Check intersection robot with wall
bool WorldModel::checkCollision(QPainterPath const &robotPath, int stroke) const
{
	QPainterPathStroker wallPathStroker;
	wallPathStroker.setWidth(stroke);
	QPainterPath const wallPath = buildWallPath();
	QPainterPath const wallStrokedPath = stroke
			? wallPathStroker.createStroke(wallPath)
			: wallPath;

	return wallStrokedPath.intersects(robotPath);
}

/// Returns all walls
QList<items::WallItem *> const &WorldModel::walls() const
{
	return mWalls;
}

void WorldModel::addWall(items::WallItem *wall)
{
	mWalls.append(wall);
	emit wallAdded(wall);
}

void WorldModel::removeWall(items::WallItem *wall)
{
	mWalls.removeOne(wall);
	emit itemRemoved(wall);
}

QList<items::ColorFieldItem *> const &WorldModel::colorFields() const
{
	return mColorFields;
}

int WorldModel::wallsCount() const
{
	return mWalls.count();
}

items::WallItem *WorldModel::wallAt(int index) const
{
	return mWalls[index];
}

void WorldModel::addColorField(items::ColorFieldItem *colorField)
{
	mColorFields.append(colorField);
	emit colorItemAdded(colorField);
}

void WorldModel::removeColorField(items::ColorFieldItem *colorField)
{
	mColorFields.removeOne(colorField);
	emit itemRemoved(colorField);
}

void WorldModel::clear()
{
	while (!mWalls.isEmpty()) {
		removeWall(mWalls.last());
	}

	while (!mColorFields.isEmpty()) {
		removeColorField(mColorFields.last());
	}
}

QPainterPath WorldModel::buildWallPath() const
{
	/// @todo Maintain a cache for this.
	QPainterPath wallPath;

	foreach (items::WallItem *wall, mWalls) {
		wallPath.moveTo(wall->begin());
		wallPath.lineTo(wall->end());
	}

	return wallPath;
}

/// Save world to xml
QDomElement WorldModel::serialize(QDomDocument &document, QPointF const &topLeftPicture) const
{
	QDomElement result = document.createElement("world");

	QDomElement walls = document.createElement("walls");
	result.appendChild(walls);
	for (items::WallItem *wall : mWalls) {
		QDomElement wallNode = wall->serialize(document, topLeftPicture.toPoint());
		walls.appendChild(wallNode);
	}

	QDomElement colorFields = document.createElement("colorFields");
	result.appendChild(colorFields);
	for (items::ColorFieldItem *colorField : mColorFields) {
		QDomElement colorFiedlNode = colorField->serialize(document, topLeftPicture.toPoint());
		colorFields.appendChild(colorFiedlNode);
	}

	return result;
}

void WorldModel::deserialize(QDomElement const &element)
{
	if (element.isNull()) {
		/// @todo Report error
		return;
	}

	clear();

	QDomNodeList allWalls = element.elementsByTagName("walls");
	for (int i = 0; i < allWalls.count(); ++i) {
		QDomElement const wallsNode = allWalls.at(i).toElement();

		QDomNodeList walls = wallsNode.elementsByTagName("wall");
		for (int i = 0; i < walls.count(); ++i) {
			QDomElement const wallNode = walls.at(i).toElement();
			items::WallItem *wall = new items::WallItem(QPointF(0, 0), QPointF(0, 0));
			wall->deserialize(wallNode);
			addWall(wall);
		}
	}

	QDomNodeList colorFields = element.elementsByTagName("colorFields");
	for (int i = 0; i < colorFields.count(); ++i) {
		QDomElement const colorFieldNode = colorFields.at(i).toElement();

		QDomNodeList ellipses = colorFieldNode.elementsByTagName("ellipse");
		for (int i = 0; i < ellipses.count(); ++i) {
			QDomElement const ellipseNode = ellipses.at(i).toElement();
			items::EllipseItem *ellipseItem = new items::EllipseItem(QPointF(0, 0), QPointF(0, 0));
			ellipseItem->deserialize(ellipseNode);
			addColorField(ellipseItem);
		}

		QDomNodeList lines = colorFieldNode.elementsByTagName("line");
		for (int i = 0; i < lines.count(); ++i) {
			QDomElement const lineNode = lines.at(i).toElement();
			items::LineItem* lineItem = new items::LineItem(QPointF(0, 0), QPointF(0, 0));
			lineItem->deserialize(lineNode);
			addColorField(lineItem);
		}

		QDomNodeList styluses = colorFieldNode.elementsByTagName("stylus");
		for (int i = 0; i < styluses.count(); ++i) {
			QDomElement const stylusNode = styluses.at(i).toElement();
			items::StylusItem *stylusItem = new items::StylusItem(0, 0);
			stylusItem->deserialize(stylusNode);
			addColorField(stylusItem);
		}
	}
}
