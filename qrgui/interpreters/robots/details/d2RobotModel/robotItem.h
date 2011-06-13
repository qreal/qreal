#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const qreal robotWidth = 50;
const qreal robotHeight = 50;

/** @brief Class that represents a robot in 2D model */
class RobotItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	RobotItem();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);

	QRectF boundingRect() const;

private:
	QImage mImage;
};

}
}
}
}
}
