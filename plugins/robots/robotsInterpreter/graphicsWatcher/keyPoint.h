#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QRadialGradient>

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace sensorsGraph
{

//! @class KeyPoint It's yellow circle on plot, source of points
class KeyPoint : public QGraphicsItem
{
public:
	KeyPoint();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	QRectF mVisualArea;
};

}
}
}
}
