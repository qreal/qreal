#pragma once

#include <QtCore/QString>
#include <QtCore/QPointF>
#include <QtCore/QList>

namespace qReal {
namespace gestures {

class IKeyManager
{
public:
	virtual QString getKey(QList<QPoint> const &path) = 0;
};

}
}
