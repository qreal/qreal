#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QPainter>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/// Class that represents line that allows to rotate QGraphicItems
class Rotater : public QObject, public QGraphicsLineItem {
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	Rotater();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);
	QRectF boundingRect() const;

	void setMasterItem(QGraphicsItem *masterItem);

private:
	QPointF mFrom;
	QPointF mTo;
	QGraphicsItem *mMaster;
};

}
}
}
}
}
