#pragma once

#include <QtGui/QGraphicsScene>
#include "../../../../../qrutils/graphicsUtils/abstractScene.h"

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

signals:
	void mouseClicked(QGraphicsSceneMouseEvent *mouseEvent);
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
