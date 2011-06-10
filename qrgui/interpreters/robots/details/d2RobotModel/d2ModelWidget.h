#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QPolygonF>

#include "robotDrawer.h"
#include "worldDrawer.h"
#include "worldModel.h"
#include "iConfigurableModel.h"
#include "d2ModelScene.h"

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

namespace drawingAction {
enum DrawingAction {
	none,
	wall
};
}

class D2ModelWidget : public QWidget {
	Q_OBJECT

public:
	explicit D2ModelWidget(IConfigurableRobotModel *robotModel, WorldModel *worldModel, QWidget *parent = 0);
	~D2ModelWidget();
	void init();
	void close();
	void draw(QPointF newCoord, qreal angle, QPointF dPoint);
	void drawBeep(QColor const &color);
	QPolygonF const robotBoundingPolygon(QPointF const &coord, qreal const &angle) const;

private slots:
	void addWall(bool on);
	void mouseClicked(QPointF const &position);

private:
	void drawWalls();

	Ui::D2Form *mUi;
	D2ModelScene *mScene;
	QGraphicsRectItem *mRobot;
	QPolygonF mLine;
	QGraphicsPolygonItem *mPolygon;
	IConfigurableRobotModel *mRobotModel;
	RobotDrawer mRobotDrawer;
	WorldDrawer mWorldDrawer;
	WorldModel *mWorldModel;

	drawingAction::DrawingAction mDrawingAction;
	int mMouseClicksCount;
	QList<QPointF> mCurrentWall;
};

}
}
}
}
}
