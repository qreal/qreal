#include "worldModel.h"

#include <QtGui/QPainterPath>

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots::details::d2Model;

WorldModel::WorldModel()
{
	mWalls << qMakePair(QPoint(50, -100), QPoint(50, 100));
}

int WorldModel::sonarReading(QPoint const &position, qreal direction) const
{
	qDebug() << "Reading sonar";
	return 0;
}

bool WorldModel::touchSensorReading(QPoint const &position, qreal direction) const
{
	QPainterPathStroker pathStroker;
	pathStroker.setWidth(1);

	typedef QPair<QPoint, QPoint> Wall;
	foreach (Wall const wall, mWalls) {
		QPainterPath wallPath;
		wallPath.moveTo(wall.first);
		wallPath.lineTo(wall.second);
		wallPath = pathStroker.createStroke(wallPath);
		if (wallPath.contains(position)) {
			return true;
		}
	}

	return false;
}

int WorldModel::colorSensorReading(QPoint const &position, sensorType::SensorTypeEnum mode) const
{
	qDebug() << "Reading color sensor";
	return 0;
}

bool WorldModel::checkCollision(QPolygonF const &robotRegion) const
{
	QPainterPath robotPath;
	robotPath.addPolygon(robotRegion);
	QPainterPathStroker pathStroker;
	pathStroker.setWidth(1);
	robotPath = pathStroker.createStroke(robotPath);

	typedef QPair<QPoint, QPoint> Wall;
	foreach (Wall const wall, mWalls) {
		QPainterPath wallPath;
		wallPath.moveTo(wall.first);
		wallPath.lineTo(wall.second);
		wallPath = pathStroker.createStroke(wallPath);
		if (wallPath.intersects(robotPath)) {
			return true;
		}
	}

	return false;
}

QList<QPair<QPoint, QPoint> > const &WorldModel::walls() const
{
	return mWalls;
}

void WorldModel::addWall(QPoint const &begin, QPoint const &end)
{
	mWalls << qMakePair(begin, end);
}
