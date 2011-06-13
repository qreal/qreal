#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QPolygonF>
#include <QtGui/QGraphicsSceneMouseEvent>

#include "robotDrawer.h"
#include "worldDrawer.h"
#include "worldModel.h"
#include "iConfigurableModel.h"
#include "d2ModelScene.h"
#include "robotItem.h"

namespace Ui {
class D2Form;
}

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace d2Model {

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

	/** @brief Get current scene position of mRobot */
	QPointF robotPos();

public slots:
	void update();

private slots:
	void addWall(bool on);
	void clearScene();
	void resetButtons();

	void mouseClicked(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);


private:
	void drawWalls();
	void drawInitialRobot();

	Ui::D2Form *mUi;
	D2ModelScene *mScene;
	RobotItem *mRobot;
	QPolygonF mLine;
	QGraphicsPolygonItem *mPolygon;
	IConfigurableRobotModel *mRobotModel;
	RobotDrawer mRobotDrawer;
	WorldDrawer mWorldDrawer;
	WorldModel *mWorldModel;

	drawingAction::DrawingAction mDrawingAction;
	int mMouseClicksCount;
	QList<QPointF> mCurrentWall;

	bool mIsBeingDragged;
	qreal mAngleOld;
	QPointF mRotatePointOld;
};

}
}
}
}
}
