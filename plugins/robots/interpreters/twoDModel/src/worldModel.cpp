#include <QtGui/QTransform>
#include <QtCore/QStringList>

#include "worldModel.h"

//#include "../tracer.h"
#include "stylusItem.h"
#include "ellipseItem.h"

using namespace twoDModel;

WorldModel::WorldModel()
{
}

int WorldModel::sonarReading(QPointF const &position, qreal direction) const
{
	int maxSonarRangeCms = 255;
	int minSonarRangeCms = 0;
	int currentRangeInCm = (minSonarRangeCms + maxSonarRangeCms) / 2;

	QPainterPath const wallPath = buildWallPath();
	if (!checkSonarDistance(maxSonarRangeCms, position, direction, wallPath)) {
//		Tracer::debug(tracer::enums::d2Model, "WorldModel::sonarReading", "Sonar sensor. Reading: max (" + QString(maxSonarRangeCms) + ")");
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

//	Tracer::debug(tracer::enums::d2Model, "WorldModel::sonarReading", "Sonar sensor. Reading: " + QString(currentRangeInCm));
	return currentRangeInCm;
}

bool WorldModel::checkSonarDistance(int const distance, QPointF const &position
		, qreal const direction, QPainterPath const &wallPath) const
{
	QPainterPath const rayPath = sonarScanningRegion(position, direction, distance);
	return rayPath.intersects(wallPath);
}

//bool WorldModel::touchSensorReading(QPointF const &position, qreal direction, robots::enums::inputPort::InputPortEnum const port)
//{
//	Q_UNUSED(direction)

//	QPainterPathStroker pathStroker;
//	pathStroker.setWidth(3);
//	QPainterPath robotPath;

//	robotPath.moveTo(mTouchSensorPositionOld[port]);
//	robotPath.lineTo(position);

//	mTouchSensorPositionOld[port] = position;

//	QPainterPath wallPath = buildWallPath();
//	return wallPath.intersects(robotPath);
//}

QPainterPath WorldModel::sonarScanningRegion(QPointF const &position, int range) const
{
	return sonarScanningRegion(position, 0, range);
}

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

QList<WallItem *> const &WorldModel::walls() const
{
	return mWalls;
}

void WorldModel::addWall(WallItem* wall)
{
	mWalls.append(wall);
}

void WorldModel::removeWall(WallItem* wall)
{
	mWalls.removeOne(wall);
}

QList<ColorFieldItem *> const &WorldModel::colorFields() const
{
	return mColorFields;
}

int WorldModel::wallsCount() const
{
	return mWalls.count();
}

WallItem *WorldModel::wallAt(int index) const
{
	return mWalls[index];
}

void WorldModel::addColorField(ColorFieldItem *colorField)
{
	mColorFields.append(colorField);
}

void WorldModel::removeColorField(ColorFieldItem *colorField)
{
	mColorFields.removeOne(colorField);
}

void WorldModel::clearScene()
{
	mWalls.clear();
	mColorFields.clear();
}

QPainterPath WorldModel::buildWallPath() const
{
	// TODO: Maintain a cache for this.
	QPainterPath wallPath;

	foreach (WallItem* wall, mWalls) {
		wallPath.moveTo(wall->begin());
		wallPath.lineTo(wall->end());
	}

	return wallPath;
}

QDomElement WorldModel::serialize(QDomDocument &document, QPointF const &topLeftPicture) const
{
	QDomElement result = document.createElement("world");

	QDomElement walls = document.createElement("walls");
	result.appendChild(walls);
	foreach (WallItem *wall, mWalls) {
		QDomElement wallNode = wall->serialize(document, topLeftPicture.toPoint());
		walls.appendChild(wallNode);
	}

	QDomElement colorFields = document.createElement("colorFields");
	result.appendChild(colorFields);
	foreach (ColorFieldItem *colorField, mColorFields) {
		QDomElement colorFiedlNode = colorField->serialize(document, topLeftPicture.toPoint());
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
	clearScene();

	QDomNodeList allWalls = element.elementsByTagName("walls");
	for (int i = 0; i < allWalls.count(); ++i) {
		QDomElement const wallsNode = allWalls.at(i).toElement();

		QDomNodeList walls = wallsNode.elementsByTagName("wall");
		for (int i = 0; i < walls.count(); ++i) {
			QDomElement const wallNode = walls.at(i).toElement();
			WallItem *wall = new WallItem(QPointF(0, 0), QPointF(0, 0));
			wall->deserialize(wallNode);
			mWalls.append(wall);
		}
	}

	QDomNodeList colorFields = element.elementsByTagName("colorFields");
	for (int i = 0; i < colorFields.count(); ++i) {
		QDomElement const colorFieldNode = colorFields.at(i).toElement();

		QDomNodeList ellipses = colorFieldNode.elementsByTagName("ellipse");
		for (int i = 0; i < ellipses.count(); ++i) {
			QDomElement const ellipseNode = ellipses.at(i).toElement();
			EllipseItem* ellipseItem = new EllipseItem(QPointF(0, 0), QPointF(0, 0));
			ellipseItem->deserialize(ellipseNode);
			mColorFields.append(ellipseItem);
		}

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
