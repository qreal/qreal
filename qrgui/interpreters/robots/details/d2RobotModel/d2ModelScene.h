#pragma once

#include <QGraphicsScene>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

/** @brief Implementation of QGraphicsScene for 2D robot model
*/
class D2ModelScene: public QGraphicsScene
{
	Q_OBJECT
public:
	D2ModelScene(QObject *parent = 0);
	virtual ~D2ModelScene();

signals:
	void mouseClicked(QPointF const &point);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};


}
}
}
}
}
