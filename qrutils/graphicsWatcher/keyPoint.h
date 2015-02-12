#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtGui/QPainter>
#include <QtGui/QRadialGradient>

namespace utils {
namespace sensorsGraph {

//! @class KeyPoint It's circle on plot
class KeyPoint : public QGraphicsItem
{
public:
	explicit KeyPoint(const QColor color);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
	QRectF mVisualArea;
	const QColor mColor;
};

}
}
