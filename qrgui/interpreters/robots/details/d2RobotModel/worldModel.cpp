#include "worldModel.h"

using namespace qReal::interpreters::robots::details::d2Model;

int WorldModel::sonarReading(QPoint const &position, qreal direction) const
{
	return 0;
}

bool WorldModel::touchSensorReading(QPoint const &position, qreal direction) const
{
	return false;
}

int WorldModel::colorSensorReading(QPoint const &position, sensorType::SensorTypeEnum mode) const
{
	return 0;
}

bool WorldModel::checkCollision(QPolygon const &robotRegion) const
{
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
