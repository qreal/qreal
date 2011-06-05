#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsRectItem>

#include "robotDrawer.h"
#include "worldDrawer.h"
#include "worldModel.h"
#include "iConfigurableModel.h"

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

class D2ModelWidget : public QWidget {
	Q_OBJECT

public:
	explicit D2ModelWidget(IConfigurableRobotModel *robotModel, WorldModel *worldModel, QWidget *parent = 0);
	~D2ModelWidget();
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
	IConfigurableRobotModel *mRobotModel;
	RobotDrawer mRobotDrawer;
	WorldDrawer mWorldDrawer;
	WorldModel *mWorldModel;
};

}
}
}
}
}
