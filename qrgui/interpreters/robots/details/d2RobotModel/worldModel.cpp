#include "worldModel.h"

#include <QtGui/QPainterPath>

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots::details::d2Model;

WorldModel::WorldModel()
{
	mWalls << qMakePair(QPointF(50, -100), QPointF(50, 100));
}

int WorldModel::sonarReading(QPoint const &position, qreal direction) const
{
	Q_UNUSED(position)
	Q_UNUSED(direction)
	qDebug() << "Reading sonar";
	return 0;
}

bool WorldModel::touchSensorReading(QPoint const &position, qreal direction) const
{
	Q_UNUSED(position)
	Q_UNUSED(direction)
	QPainterPathStroker pathStroker;
	pathStroker.setWidth(1);

	typedef QPair<QPointF, QPointF> Wall;
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
	Q_UNUSED(mode)
	Q_UNUSED(position)
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

	typedef QPair<QPointF, QPointF> Wall;
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

QList<QPair<QPointF, QPointF> > const &WorldModel::walls() const
{
	return mWalls;
}

void WorldModel::addWall(QPointF const &begin, QPointF const &end)
{
	mWalls << qMakePair(begin, end);
}
