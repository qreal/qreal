#pragma once

#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include "../../../../../qrutils/graphicsUtils/abstractScene.h"
#include "../../../../../qrutils/graphicsUtils/griddrawer.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/// Implementation of QGraphicsScene for 2D robot model
class D2ModelScene: public graphicsUtils::AbstractScene
{
	Q_OBJECT
public:
	D2ModelScene(graphicsUtils::AbstractView *view, QObject *parent = 0);
	virtual ~D2ModelScene();

	void keyPressEvent(QKeyEvent *event);

	void forPressResize(QGraphicsSceneMouseEvent *event);
	void forMoveResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void forReleaseResize(QGraphicsSceneMouseEvent *event, QRectF const &rect);
	void reshapeItem(QGraphicsSceneMouseEvent *event, QRectF const &rect);
    void drawBackground ( QPainter * painter, const QRectF & rect );
    graphicsUtils::GridDrawer *gd;

signals:
	void mousePressed(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);
	void itemDeleted(QGraphicsItem *);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
};


}
}
}
}
}
