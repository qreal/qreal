#pragma once
#include <QtGui/QGraphicsItem>
#include "colorFieldItem.h"
#include "../../../../../qrutils/graphicsUtils/rectangleImpl.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

class EjectedItem : public QObject, public graphicsUtils::AbstractItem
{
	Q_OBJECT
public:
	EjectedItem(QPointF begin, QPointF end);
	virtual QRectF calcNecessaryBoundingRect() const;
	virtual QRectF boundingRect() const;
	virtual void drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual void drawFieldForResizeItem(QPainter* painter);

	virtual QDomElement serialize(QDomDocument &document, QPoint const &topLeftPicture);
	virtual void deserialize(QDomElement const &element);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

	bool isDragged();

public slots:
	void robotChangedPosition(QRectF const &newRect, QPointF const& diffPos);
	void toStopMovedEjectedItem(bool isNeedStop, QPointF const& oldPos);
	void toStopDraggedEjectedItem(bool isNeedStop, QPointF const& oldPos);

signals:
	void ejectedItemMoved(QRectF const &itemRect, QPointF const &oldPos, QPointF const& diffRobotPos);
	void ejectedItemDragged(QRectF const &itemRect, QPointF const &oldPos);

protected:
	graphicsUtils::RectangleImpl mRectImpl;
	void setPrivateData();
	void toStopEjectedItem(bool isNeedStop, QPointF const& oldPos);

private:
	bool mDragged;
	bool mMoved;
	QString mSerializeName;
};
}
}
}
}
}
