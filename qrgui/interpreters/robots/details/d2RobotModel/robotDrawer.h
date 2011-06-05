#pragma once
#include <QtGui/QWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsRectItem>

namespace Ui {
	class D2Form;
}

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

const qreal robotWidth = 50;
const qreal robotHeight = 50;

class RobotDrawer : public QWidget {
	Q_OBJECT
public:
	explicit RobotDrawer(QWidget *parent = 0);
	~RobotDrawer();
	void init();
	void close();
	void draw(QPointF newCoord, qreal angle, QPointF dPoint);
	void drawBeep(QColor const &color);
private:
	Ui::D2Form *mUi;
	QGraphicsScene *mScene;
	QGraphicsRectItem *mRobot;
	QPolygonF mLine;
	QGraphicsPolygonItem *mPolygon;
};

}
}
}
}
}
