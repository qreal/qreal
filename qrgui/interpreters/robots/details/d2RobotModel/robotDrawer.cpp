#include "robotDrawer.h"

using namespace qReal::interpreters::robots::details::d2Model;

void RobotDrawer::draw(QPointF newCoord, qreal angle, QPointF dPoint, QPainter &painter)
{
	Q_UNUSED(newCoord)
	Q_UNUSED(angle)
	Q_UNUSED(dPoint)
	Q_UNUSED(painter)
}

void RobotDrawer::drawBeep(QColor const &color, QPainter &painter)
{
	Q_UNUSED(color)
	Q_UNUSED(painter)
}
