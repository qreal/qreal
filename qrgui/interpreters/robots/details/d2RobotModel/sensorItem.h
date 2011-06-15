#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/** @brief Class that represents robot port in 2D model */
class SensorItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	SensorItem(QColor const &color);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);

	QRectF boundingRect() const;

	void mousePressEvent(QGraphicsSceneMouseEvent * event);

	void move(qreal dx, qreal dy);

private:
	QColor mColor;
	QRectF mRectangle;
};

}
}
}
}
}
