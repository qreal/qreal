#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtGui/QPolygonF>
#include <QtCore/QSignalMapper>
#include <QtCore/QHash>

#include <qrutils/qRealDialog.h>
#include <qrutils/graphicsUtils/lineImpl.h>

#include "lineItem.h"
#include "stylusItem.h"
#include "ellipseItem.h"
#include "worldModel.h"
#include "src/engine/twoDRobotModelInterface.h"
#include "d2ModelScene.h"
#include "robotItem.h"
#include "rotater.h"
#include "timeline.h"

#include "commonTwoDModel/engine/configurer.h"
#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"

#include <interpreterBase/devicesConfigurationProvider.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

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

class D2ModelWidget : public utils::QRealDialog, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	D2ModelWidget(TwoDRobotRobotModelInterface *twoDRobotModel, WorldModel *worldModel
			, interpreterBase::robotModel::RobotModelInterface &robotModel
			, engine::TwoDModelDisplayWidget *display
			, Configurer const &configurer
			, QWidget *parent = 0);

	~D2ModelWidget();
	void init(bool isActive = true);
	void close();
	void draw(QPointF const &newCoord, qreal angle);
	void drawBeep(bool isNeededBeep);
	QPainterPath const robotBoundingPolygon(QPointF const &coord, qreal const &angle) const;

	/// Get current scene position of robot
	QPointF robotPos() const;

	/// Returns false if we clicked on robot and are moving it somewhere
	bool isRobotOnTheGround();

	/// Enables Run and Stop buttons
	void enableRunStopButtons();

	/// Disables Run and Stop buttons, used when current tab is not related to robots
	void disableRunStopButtons();

	D2ModelScene* scene();

	void setSensorVisible(interpreterBase::robotModel::PortInfo const &port, bool isVisible);

	void closeEvent(QCloseEvent *event);

	SensorItem *sensorItem(interpreterBase::robotModel::PortInfo const &port);

	void loadXml(QDomDocument const &worldModel);

	void setRunStopButtonsEnabled(bool enabled);

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

	void runButtonPressed();

	void stopButtonPressed();

protected:
	virtual void changeEvent(QEvent *e);
	virtual void showEvent(QShowEvent *e);
	virtual void keyPressEvent(QKeyEvent *event);

	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, const interpreterBase::robotModel::DeviceInfo &device) override;

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

	void addPort(int const index);

	void handleNewRobotPosition();

	void saveToRepo();
	void saveWorldModel();
	void loadWorldModel();

	void changePenWidth(int width);
	void changePenColor(int textIndex);
	void changePalette();

	void changeSpeed(int curIndex);

	void enableRobotFollowing(bool on);
	void onHandCursorButtonToggled(bool on);
	void onMultiselectionCursorButtonToggled(bool on);
	void setCursorType(enums::cursorType::CursorType cursor);

	void alignWalls();
	void changePhysicsSettings();

	void onTimelineTick();

	void toggleDisplayVisibility();

	void rereadDevicesConfiguration();

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

	void connectUiButtons();
	void initButtonGroups();
	void initPorts();
	bool isPortConfigurable(interpreterBase::robotModel::PortInfo const &port);
	void setHighlightOneButton(QAbstractButton * const oneButton);

	void drawWalls();
	void drawColorFields();
	void drawInitialRobot();

	void setDisplayVisibility(bool visible);

	QDomDocument generateXml() const;

	/// Set active panel toggle button and deactivate all others
	void setActiveButton(int active);

	/// Get QPushButton for current sensor
	QPushButton *currentPortButton();

	/// Deletes sensor for given port and removes it from scene and the robot
	void removeSensor(interpreterBase::robotModel::PortInfo const &port);

	/// Reread sensor configuration on given port, delete old sensor item and create new.
	void reinitSensor(interpreterBase::robotModel::PortInfo const &port);

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

	void centerOnRobot();
	QGraphicsView::DragMode cursorTypeToDragType(enums::cursorType::CursorType type) const;
	QCursor cursorTypeToCursor(enums::cursorType::CursorType type) const;
	void processDragMode();
	void syncCursorButtons();

	void onFirstShow();

	void initRunStopButtons();

	void updateWheelComboBoxes();

	Ui::D2Form *mUi;
	D2ModelScene *mScene;
	RobotItem *mRobot;

	/// Maximum number of calls to draw() when adding robot path element is skipped.
	/// So, new path element is added every mMaxDrawCyclesBetweenPathElements times
	/// draw() is called. We can't do that every time due to performance issues ---
	/// robot position gets recalculated too frequently (about 10 times for single pixel of a movement).
	int mMaxDrawCyclesBetweenPathElements;

	TwoDRobotRobotModelInterface *mTwoDRobotModel;
	WorldModel *mWorldModel;
	engine::TwoDModelDisplayWidget *mDisplay;

	/// Current action (toggled button on left panel)
	enums::drawingAction::DrawingAction mDrawingAction;

	/// Variable to count clicks on scene, used to create walls
	int mMouseClicksCount;

	/// Temporary wall that's being created. When it's complete, it's added to world model
	WallItem *mCurrentWall;
	LineItem *mCurrentLine;
	StylusItem *mCurrentStylus;
	EllipseItem *mCurrentEllipse;

	/// Signal mapper for handling addPortButtons' clicks.
	QSignalMapper mPortsMapper;

	/// Current port that we're trying to add to 2D model scene.
	interpreterBase::robotModel::PortInfo mCurrentPort;

	/// Type of current sensor that we add.
	interpreterBase::robotModel::DeviceInfo mCurrentSensorType;

	/// List of flags showing which panel button is active now.
	QList<bool> mButtonFlags;

	/// Modeled sensors.
	QHash<interpreterBase::robotModel::PortInfo, SensorItem *> mSensors;

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

	interpreterBase::robotModel::RobotModelInterface &mRobotModel;

	/// Does not have ownership, combo boxes are owned by form layout.
	QHash<QComboBox *, interpreterBase::robotModel::PortInfo> mComboBoxesToPortsMap;

	Configurer const &mConfigurer;
};

}
