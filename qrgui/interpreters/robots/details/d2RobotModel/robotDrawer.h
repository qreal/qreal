#pragma once

#include <QtCore/QPointF>
#include <QtGui/QColor>
#include <QtGui/QPainter>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class RobotDrawer
{
public:
	void draw(QPointF newCoord, qreal angle, QPointF dPoint, QPainter &painter);
	void drawBeep(QColor const &color, QPainter &painter);
};

}
}
}
}
}
