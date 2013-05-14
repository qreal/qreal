#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtGui/QPolygonF>
#include <QtCore/QSignalMapper>

#include "lineItem.h"
#include "stylusItem.h"
#include "ellipseItem.h"
#include "worldModel.h"
#include "robotModelInterface.h"
#include "d2ModelScene.h"
#include "robotItem.h"
#include "rotater.h"
#include "timeline.h"
#include "../../../../../qrutils/graphicsUtils/lineImpl.h"

namespace Ui
{
	class D2Form;
}

namespace qReal
{
namespace interpreters
{
namespace robots
{
namespace details
{
namespace d2Model
{

namespace drawingAction
{
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

namespace cursorType
{
enum CursorType
{
	NoDrag = 0
	, hand
	, multiselection
};
}

class D2ModelWidget : public QWidget {
	Q_OBJECT

public:
	D2ModelWidget(RobotModelInterface *robotModel, WorldModel *worldModel, QWidget *parent = 0);
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
	void setRobotVisible(bool isVisible);
	void setSensorVisible(inputPort::InputPortEnum port, bool isVisible);

	void closeEvent(QCloseEvent *event);

	QVector<SensorItem *> sensorItems() const;

public slots:
	void update();
	void worldWallDragged(WallItem *wall, QPainterPath const &shape, QPointF const& oldPos);
	/// Places in 2D model same sensors as selected in QReal settings
	void syncronizeSensors();
	/// Synchronizes noise settings in 2D model window with global ones
	void rereadNoiseSettings();

signals:
	void robotWasIntersectedByWall(bool isNeedStop, QPointF const& oldPos);
	/// Emitted when such features as motor or sensor noise were
	///enabled or disabled by user
	void noiseSettingsChanged();

protected:
	void changeEvent(QEvent *e);
	void showEvent(QShowEvent *e);

private slots:
	void addWall(bool on);
	void addLine(bool on);
	void addStylus(bool on);
	void addEllipse(bool on);
	void clearScene();
	void resetButtons();

	void mousePressed(QGraphicsSceneMouseEvent *mouseEvent);
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

	void changeSpeed(int curIndex);
	void changeSensorType(inputPort::InputPortEnum const port
			, sensorType::SensorTypeEnum const type);

	void enableRobotFollowing(bool on);
	void onHandCursorButtonToggled(bool on);
	void onMultiselectionCursorButtonToggled(bool on);
	void setCursorType(cursorType::CursorType cursor);

	void changeNoiseSettings();

	void startTimelineListening();
	void stopTimelineListening();
	void onTimelineTick();

signals:
	void d2WasClosed();

protected:
	virtual void keyPressEvent(QKeyEvent *event);

private:
	static const int defaultPenWidth = 15;

	static const int indexOfNoneSensor = 0;
	static const int indexOfTouchSensor = 1;
	static const int indexOfColorSensor = 2;
	static const int indexOfSonarSensor = 3;
	static const int indexOfLightSensor = 4;

	void connectUiButtons();
	void initButtonGroups();
	void setHighlightOneButton(QAbstractButton const *oneButton);

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
	void reshapeEllipse(QGraphicsSceneMouseEvent *event);

	void setValuePenColorComboBox(QColor const &penColor);
	void setValuePenWidthSpinBox(int width);
	void setItemPalette(QPen const &penItem, QBrush const &brushItem);
	void setNoPalette();

	void initWidget();
	QList<graphicsUtils::AbstractItem *> selectedColorItems();
	bool isColorItem(graphicsUtils::AbstractItem *item);

	int sensorTypeToComboBoxIndex(sensorType::SensorTypeEnum const type);

	void centerOnRobot();
	QGraphicsView::DragMode cursorTypeToDragType(cursorType::CursorType type) const;
	Qt::CursorShape cursorTypeToShape(cursorType::CursorType type) const;
	void processDragMode(int mode);
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

	/** @brief Current action (toggled button on left panel)*/
	drawingAction::DrawingAction mDrawingAction;

	/** @brief Variable to count clicks on scene, used to create walls */
	int mMouseClicksCount;

	/** @brief Temporary wall that's being created. When it's complete, it's added to world model */
	WallItem *mCurrentWall;
	LineItem *mCurrentLine;
	StylusItem *mCurrentStylus;
	EllipseItem *mCurrentEllipse;

	/** @brief Signal mapper for handling addPortButtons' clicks */
	QSignalMapper mPortsMapper;

	/** @brief Current port that we're trying to add to 2D model scene*/
	inputPort::InputPortEnum mCurrentPort;

	/** @brief Type of current sensor that we add */
	sensorType::SensorTypeEnum mCurrentSensorType;

	/** @brief List of flags showing which panel button is active now*/
	QList<bool> mButtonFlags;

	/** @brief List of sensors, index is port of sensor */
	QVector<SensorItem *> mSensors;

	int mWidth;

	bool mClearing;
	bool mRobotWasSelected;

	QButtonGroup mButtonGroup;
	QButtonGroup mCursorButtonGroup;

	cursorType::CursorType mCursorType;
	bool mFollowRobot;

	bool mFirstShow;
	Timeline const * mTimeline;
};

}
}
}
}
}
