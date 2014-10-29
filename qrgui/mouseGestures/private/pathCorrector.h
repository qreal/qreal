#pragma once

#include <QtCore/QList>
#include <QtCore/QPointF>

const qreal sense = 1;
const qreal speedKoef = 0.0275;

namespace qReal {
namespace gestures {

class PathCorrector
{
public:
	static QList<QPoint> correctPath(QList<QPoint> const &path);
	static QList<QPoint> getMousePath(QList<QPoint> const &path);
};

}
}
