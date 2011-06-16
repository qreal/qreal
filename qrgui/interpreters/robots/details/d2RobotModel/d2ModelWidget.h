#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QPolygonF>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtCore/QSignalMapper>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

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
	wall,
	port
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

	/** @brief Returns false if we click on robot and move it somewhere */
	bool isRobotOnTheGround();

public slots:
	void update();

private slots:
	void addWall(bool on);
	void clearScene();
	void resetButtons();

	void mouseClicked(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);

	void addPort(int const port);

	void saveWorldModel();
	void loadWorldModel();

private:
	void connectUiButtons();
	void drawWalls();
	void drawInitialRobot();
	/** @brief Set active panel toggle button and deactivate all others */
	void setActiveButton(int active);
	/** @brief Get QComboBox that sets current sensor's type */
	QComboBox *currentComboBox();
	/** @brief Get QPushButton for current sensor */
	QPushButton *currentPortButton();

	Ui::D2Form *mUi;
	D2ModelScene *mScene;
	RobotItem *mRobot;
	QPolygonF mLine;
	QGraphicsPolygonItem *mPolygon;
	IConfigurableRobotModel *mRobotModel;
	RobotDrawer mRobotDrawer;
	WorldDrawer mWorldDrawer;
	WorldModel *mWorldModel;

	/** @brief Current action (toggled button on left panel)*/
	drawingAction::DrawingAction mDrawingAction;
	/** @brief Variable to count clicks on scene, used to create walls */
	int mMouseClicksCount;
	/** @brief Temporary wall that's being created. When it's complete, it's added to world model */
	QList<QPointF> mCurrentWall;

	/** @brief Latest value of angle for drawing robot image */
	qreal mAngleOld;
	/** @brief Latest value of rotate point for drawing robot image */
	QPointF mRotatePointOld;

	/** @brief Signal mapper for handling addPortButtons' clicks */
	QSignalMapper mPortsMapper;
	/** @brief Current port that we're trying to add to 2D model scene*/
	inputPort::InputPortEnum mCurrentPort;
	/** @brief Type of current sensor that we add */
	sensorType::SensorTypeEnum mCurrentSensorType;
	/** @brief Color of current port item */
	QColor mCurrentPortColor;

	/** @brief Amount of buttons on left panel */
	int const mButtonsCount;
	/** @brief List of flags showing which panel button is active now*/
	QList<bool> mButtonFlags;

};

}
}
}
}
}
