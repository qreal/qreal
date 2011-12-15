#pragma once

#include <QtGui/QWidget>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QPolygonF>
#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtCore/QSignalMapper>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include "lineItem.h"
#include "stylusItem.h"
#include "worldModel.h"
#include "robotModelInterface.h"
#include "d2ModelScene.h"
#include "robotItem.h"
#include "rotater.h"
#include "../../../../../qrutils/graphicsUtils/lineImpl.h"

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
	line,
	stylus,
	port
};
}

class D2ModelWidget : public QWidget {
	Q_OBJECT

public:
	explicit D2ModelWidget(RobotModelInterface *robotModel, WorldModel *worldModel, QWidget *parent = 0);
	~D2ModelWidget();
	void init(bool isActive = true);
	void close();
	void draw(QPointF newCoord, qreal angle, QPointF dPoint);
	void drawBeep(QColor const &color);
	QPolygonF const robotBoundingPolygon(QPointF const &coord, qreal const &angle) const;

	/** @brief Get current scene position of mRobot */
	QPointF robotPos() const;

	/** @brief Returns false if we click on robot and move it somewhere */
	bool isRobotOnTheGround();

	void setD2ModelWidgetActions(QAction *runAction, QAction *stopAction);

	D2ModelScene* scene();
	void setSensorVisible(inputPort::InputPortEnum port, bool isVisible);

public slots:
	void update();

protected:
	void changeEvent(QEvent *e);

private slots:
	void addWall(bool on);
	void addLine(bool on);
	void addStylus(bool on);
	void clearScene();
	void resetButtons();

	void mouseClicked(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);

	void deleteItem(QGraphicsItem *);

	void addPort(int const port);

	void handleNewRobotPosition();

	void saveWorldModel();
	void loadWorldModel();

	void changePenWidth(int width);
	void changePenColor(const QString &text);
	void changePalette();

private:
	Ui::D2Form *mUi;
	D2ModelScene *mScene;
	RobotItem *mRobot;

	/// Holds graphic items that represent path of a robot, to be able to manipulate them
	QList<QGraphicsItem *> mRobotPath;

	/// Counter of calls to draw() method which recalculates scene position of the robot,
	/// to support robot path drawing
	int mDrawCyclesCount;

	/// Maximum number of calls to draw() when adding robot path element is skipped.
	/// So, new path element is added every mMaxDrawCyclesBetweenPathElements times
	/// draw() is called. We can't do that every time due to performance issues ---
	/// robot position gets recalculated too frequently (about 10 times for single pixel of a movement).
	int mMaxDrawCyclesBetweenPathElements;

	RobotModelInterface *mRobotModel;
	WorldModel *mWorldModel;

	/** @brief Current action (toggled button on left panel)*/
	drawingAction::DrawingAction mDrawingAction;

	/** @brief Variable to count clicks on scene, used to create walls */
	int mMouseClicksCount;

	/** @brief Temporary wall that's being created. When it's complete, it's added to world model */
	WallItem* mCurrentWall;
	LineItem* mCurrentLine;
	StylusItem* mCurrentStylus;

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

	/** @brief Amount of buttons on left panel */
	int const mButtonsCount;

	/** @brief List of flags showing which panel button is active now*/
	QList<bool> mButtonFlags;

	/** @brief List of sensors, index is port of sensor */
	QVector<SensorItem *> mSensors;

	Rotater *mRotater;
	QVector<Rotater *> mSensorRotaters;

	void connectUiButtons();
	void drawWalls();
	void drawColorFields();
	void drawInitialRobot();

	/** @brief Set active panel toggle button and deactivate all others */
	void setActiveButton(int active);

	/** @brief Get QComboBox that sets current sensor's type */
	QComboBox *currentComboBox();

	/** @brief Get QPushButton for current sensor */
	QPushButton *currentPortButton();

	/** Deletes sensor for given port and removes it from scene and the robot */
	void removeSensor(inputPort::InputPortEnum port);

	/// Adds sensor to a scene and a robot
	void addSensor(inputPort::InputPortEnum port, sensorType::SensorTypeEnum type);

	/// Reread sensor configuration on given port, delete old sensor item and create new.
	void reinitSensor(inputPort::InputPortEnum port);

	void reshapeWall(QGraphicsSceneMouseEvent *event);
	void reshapeLine(QGraphicsSceneMouseEvent *event);
	void reshapeStylus(QGraphicsSceneMouseEvent *event);

		void setValuePenColorComboBox(QColor penColor);
		void setValuePenWidthSpinBox(int width);
	void setItemPalette(QPen const &penItem, QBrush const &brushItem);
	void setNoPalette();

	void initWidget();
	QList<graphicsUtils::AbstractItem *> selectedColorItems();
	bool isColorItem(graphicsUtils::AbstractItem *item);
};

}
}
}
}
}
