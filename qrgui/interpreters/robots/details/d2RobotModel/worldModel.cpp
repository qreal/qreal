#include "worldModel.h"

#include <QtGui/QTransform>

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots::details::d2Model;

WorldModel::WorldModel()
{
	mWalls << qMakePair(QPointF(100, -100), QPointF(100, 100));
}

int WorldModel::sonarReading(QPoint const &position, qreal direction) const
{
	int const maxSonarRangeCms = 255;

	QPainterPath const wallPath = buildWallPath();

	for (int currentRangeInCm = 1; currentRangeInCm <= maxSonarRangeCms; ++currentRangeInCm) {
		QPainterPath rayPath = sonarScanningRegion(position, direction, currentRangeInCm);
		if (rayPath.intersects(wallPath)) {
			qDebug() << "Sonar sensor. Reading: " << currentRangeInCm;
			return currentRangeInCm;
		}
	}

	qDebug() << "Sonar sensor. Reading: max (" << maxSonarRangeCms << ")";
	return maxSonarRangeCms;
}

bool WorldModel::touchSensorReading(QPoint const &position, qreal direction)
{
	Q_UNUSED(direction)
	QPainterPathStroker pathStroker;
	pathStroker.setWidth(3);
	QPainterPath robotPath;
	robotPath.moveTo(mTouchSensorPositionOld);
	mTouchSensorPositionOld = position;
	robotPath.lineTo(position);

	QPainterPath wallPath = buildWallPath();
	return wallPath.intersects(robotPath);
}

int WorldModel::colorSensorReading(QPoint const &position, sensorType::SensorTypeEnum mode) const
{
	Q_UNUSED(mode)
	Q_UNUSED(position)
	qDebug() << "Reading color sensor";
	return 0;
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

QList<QPair<QPointF, QPointF> > const &WorldModel::walls() const
{
	return mWalls;
}

void WorldModel::addWall(QPointF const &begin, QPointF const &end)
{
	mWalls << qMakePair(begin, end);
}

void WorldModel::clearScene()
{
	mWalls.clear();
}

QPainterPath WorldModel::buildWallPath() const
{
	// TODO: Maintain a cache for this.
	QPainterPathStroker pathStroker;
	pathStroker.setWidth(3);
	QPainterPath wallPath;

	typedef QPair<QPointF, QPointF> Wall;
	foreach (Wall const wall, mWalls) {
		wallPath.moveTo(wall.first);
		wallPath.lineTo(wall.second);
	}
	return wallPath;
}
