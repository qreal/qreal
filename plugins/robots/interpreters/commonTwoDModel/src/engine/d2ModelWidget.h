#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtGui/QPolygonF>
#include <QtCore/QSignalMapper>

#include <qrutils/qRealDialog.h>
#include <qrutils/graphicsUtils/lineImpl.h>

#include "lineItem.h"
#include "stylusItem.h"
#include "ellipseItem.h"
#include "worldModel.h"
#include "src/engine/robotModelInterface.h"
#include "d2ModelScene.h"
#include "robotItem.h"
#include "rotater.h"
#include "timeline.h"
//#include "../nxtDisplay.h"

#include <interpreterBase/sensorsConfigurationProvider.h>

namespace Ui {
class D2Form;
}

namespace twoDModel {

namespace enums {

namespace drawingAction {
enum DrawingAction
{
	none = 0
	, wall
	, line
	, stylus
	, Port
	, ellipse
	, noneWordLoad
};
}

namespace cursorType {
enum CursorType
{
	NoDrag = 0
	, hand
	, multiselection
	, drawWall
	, drawLine
	, drawStylus
	, drawEllipse
};
}

}

class D2ModelWidget : public utils::QRealDialog, public interpreterBase::SensorsConfigurationProvider
{
	Q_OBJECT

public:
	D2ModelWidget(RobotModelInterface *robotModel, WorldModel *worldModel
			/*, NxtDisplay *nxtDisplay*/, QWidget *parent = 0);
	~D2ModelWidget();
	void init(bool isActive = true);
	void close();
	void draw(QPointF const &newCoord, qreal angle);
	void drawBeep(bool isNeededBeep);
	QPainterPath const robotBoundingPolygon(QPointF const &coord, qreal const &angle) const;

	/// Get current scene position of mRobot
	QPointF robotPos() const;

	/// Returns false if we click on robot and move it somewhere
	bool isRobotOnTheGround();

	void setD2ModelWidgetActions(QAction *runAction, QAction *stopAction);

	/// Enables Run and Stop buttons
	void enableRunStopButtons();

	/// Disables Run and Stop buttons, used when current tab is not related to robots
	void disableRunStopButtons();

	D2ModelScene* scene();
//	void setSensorVisible(robots::enums::inputPort::InputPortEnum port, bool isVisible);

	void closeEvent(QCloseEvent *event);

	QVector<SensorItem *> sensorItems() const;

	void loadXml(QDomDocument const &worldModel);

public slots:
	void update();
	void worldWallDragged(WallItem *wall, QPainterPath const &shape, QPointF const& oldPos);

	/// Starts 2D model time counter
	void startTimelineListening();

	/// Stops 2D model time counter
	void stopTimelineListening();
	void saveInitialRobotBeforeRun();
	void setInitialRobotBeforeRun();

signals:
	void d2WasClosed();

	void robotWasIntersectedByWall(bool isNeedStop, QPointF const& oldPos);

	/// Emitted when such features as motor or sensor noise were
	///enabled or disabled by user
	void noiseSettingsChanged();

	/// Emitted each time when some user actions lead to world model modifications
	/// @param xml World model description in xml format
	void modelChanged(QDomDocument const &xml);

	/// Emitted when sensor settings are changed in 2d model widget.
//	void sensorChanged(int port, robots::enums::sensorType::SensorTypeEnum type);

protected:
	virtual void changeEvent(QEvent *e);
	virtual void showEvent(QShowEvent *e);
	virtual void keyPressEvent(QKeyEvent *event);

	void onSensorConfigurationChanged(QString const &robotModel, interpreterBase::robotModel::PortInfo const &port
			, const interpreterBase::robotModel::DeviceInfo &sensor);

private slots:
	void addWall(bool on);
	void addLine(bool on);
	void addStylus(bool on);
	void addEllipse(bool on);
	void clearScene(bool removeRobot = false);
	void setNoneButton();
	void resetButtons();

	void mousePressed(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleased(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoved(QGraphicsSceneMouseEvent *mouseEvent);

	void deleteItem(QGraphicsItem *);

	void addPort(int const port);

	void handleNewRobotPosition();

	void saveToRepo();
	void saveWorldModel();
	void loadWorldModel();

	void changePenWidth(int width);
	void changePenColor(int textIndex);
	void changePalette();

	void changeSpeed(int curIndex);
//	void changeSensorType(robots::enums::inputPort::InputPortEnum const port
//			, robots::enums::sensorType::SensorTypeEnum const type);

	void enableRobotFollowing(bool on);
	void onHandCursorButtonToggled(bool on);
	void onMultiselectionCursorButtonToggled(bool on);
	void setCursorType(enums::cursorType::CursorType cursor);

	void alignWalls();
	void changePhysicsSettings();

	void onTimelineTick();

	void toggleDisplayVisibility();

private:
	static const int defaultPenWidth = 15;

	static const int indexOfNoneSensor = 0;
	static const int indexOfTouchSensor = 1;
	static const int indexOfColorSensor = 2;
	static const int indexOfSonarSensor = 3;
	static const int indexOfLightSensor = 4;

	struct RobotState {
		QPointF pos;
		double rotation;
	};

//	void onSensorConfigurationChanged(
//			robots::enums::inputPort::InputPortEnum port
//			, robots::enums::sensorType::SensorTypeEnum type
//			) override;

	void connectUiButtons();
	void initButtonGroups();
	void setHighlightOneButton(QAbstractButton * const oneButton);

	void drawWalls();
	void drawColorFields();
	void drawInitialRobot();

	void setDisplayVisibility(bool visible);

	QDomDocument generateXml() const;

	/// Set active panel toggle button and deactivate all others
	void setActiveButton(int active);

	/// Get QComboBox that sets current sensor's type
	QComboBox *currentComboBox();

	/// Get QPushButton for current sensor
	QPushButton *currentPortButton();

	/// Deletes sensor for given port and removes it from scene and the robot
//	void removeSensor(robots::enums::inputPort::InputPortEnum port);

	/// Adds sensor to a scene and a robot
//	void addSensor(robots::enums::inputPort::InputPortEnum port
//			, robots::enums::sensorType::SensorTypeEnum type
//			);

	/// Reread sensor configuration on given port, delete old sensor item and create new.
//	void reinitSensor(robots::enums::inputPort::InputPortEnum port);

	void reshapeWall(QGraphicsSceneMouseEvent *event);
	void reshapeLine(QGraphicsSceneMouseEvent *event);
	void reshapeStylus(QGraphicsSceneMouseEvent *event);
	void reshapeEllipse(QGraphicsSceneMouseEvent *event);

	void setValuePenColorComboBox(QColor const &penColor);
	void setValuePenWidthSpinBox(int width);
	void setItemPalette(QPen const &penItem, QBrush const &brushItem);
	void setNoPalette();

	void setNoneStatus();
	void setCursorTypeForDrawing(enums::cursorType::CursorType type);

	void initWidget();
	QList<graphicsUtils::AbstractItem *> selectedColorItems();
	bool isColorItem(graphicsUtils::AbstractItem *item);

//	int sensorTypeToComboBoxIndex(robots::enums::sensorType::SensorTypeEnum const type);

	void centerOnRobot();
	QGraphicsView::DragMode cursorTypeToDragType(enums::cursorType::CursorType type) const;
	QCursor cursorTypeToCursor(enums::cursorType::CursorType type) const;
	void processDragMode();
	void syncCursorButtons();

	void onFirstShow();

	Ui::D2Form *mUi;
	D2ModelScene *mScene;
	RobotItem *mRobot;

	/// Maximum number of calls to draw() when adding robot path element is skipped.
	/// So, new path element is added every mMaxDrawCyclesBetweenPathElements times
	/// draw() is called. We can't do that every time due to performance issues ---
	/// robot position gets recalculated too frequently (about 10 times for single pixel of a movement).
	int mMaxDrawCyclesBetweenPathElements;

	RobotModelInterface *mRobotModel;
	WorldModel *mWorldModel;
//	NxtDisplay *mNxtDisplay;

	/// Current action (toggled button on left panel)
	enums::drawingAction::DrawingAction mDrawingAction;

	/// Variable to count clicks on scene, used to create walls
	int mMouseClicksCount;

	/// Temporary wall that's being created. When it's complete, it's added to world model
	WallItem *mCurrentWall;
	LineItem *mCurrentLine;
	StylusItem *mCurrentStylus;
	EllipseItem *mCurrentEllipse;

	/// Signal mapper for handling addPortButtons' clicks
	QSignalMapper mPortsMapper;

//	/// Current port that we're trying to add to 2D model scene
//	robots::enums::inputPort::InputPortEnum mCurrentPort;

//	/// Type of current sensor that we add
//	robots::enums::sensorType::SensorTypeEnum mCurrentSensorType;

	/// List of flags showing which panel button is active now
	QList<bool> mButtonFlags;

	/// List of sensors, index is port of sensor
	QVector<SensorItem *> mSensors;

	int mWidth;

	bool mClearing;
	bool mRobotWasSelected;

	QButtonGroup mButtonGroup;
	QButtonGroup mCursorButtonGroup;

	enums::cursorType::CursorType mNoneCursorType; // cursorType for noneStatus
	enums::cursorType::CursorType mCursorType; // current cursorType
	bool mFollowRobot;

	bool mFirstShow;
	Timeline const * mTimeline;

	RobotState mInitialRobotBeforeRun;
};

}
