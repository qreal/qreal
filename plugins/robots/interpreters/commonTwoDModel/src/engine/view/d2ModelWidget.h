#pragma once

#include <QtCore/QSignalMapper>
#include <QtWidgets/QButtonGroup>

#include <qrutils/qRealDialog.h>
#include <qrutils/graphicsUtils/lineImpl.h>

#include <interpreterBase/devicesConfigurationProvider.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

#include "d2ModelScene.h"
#include "robotItem.h"
#include "rotater.h"

#include "src/engine/model/model.h"

#include "commonTwoDModel/engine/configurer.h"
#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"

class QComboBox;

namespace Ui {
class D2Form;
}

namespace twoDModel {

namespace items {
class WallItem;
class LineItem;
class StylusItem;
class EllipseItem;
}

namespace view {

class D2ModelWidget : public utils::QRealDialog, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	/// Takes ownership on configurer.
	D2ModelWidget(model::Model &model, Configurer const * const configurer, QWidget *parent = 0);
	~D2ModelWidget();

	void init();
	void close();

	/// Get current scene position of robot
	QPointF robotPos() const;

	/// Enables Run and Stop buttons
	void enableRunStopButtons();

	/// Disables Run and Stop buttons, used when current tab is not related to robots
	void disableRunStopButtons();

	D2ModelScene *scene();

	engine::TwoDModelDisplayWidget *display();

	void setSensorVisible(interpreterBase::robotModel::PortInfo const &port, bool isVisible);

	void closeEvent(QCloseEvent *event);

	SensorItem *sensorItem(interpreterBase::robotModel::PortInfo const &port);

	void loadXml(QDomDocument const &worldModel);

	/// Enables or disables interpreter control buttons.
	void setRunStopButtonsEnabled(bool enabled);

public slots:
	void update();
	void worldWallDragged(items::WallItem *wall, QPainterPath const &shape, QPointF const &oldPos);

	void saveInitialRobotBeforeRun();
	void setInitialRobotBeforeRun();

signals:
	/// Emitted each time when user closes 2D model window.
	void widgetClosed();

	void robotWasIntersectedByWall(bool isNeedStop, QPointF const &oldPos);

	/// Emitted when such features as motor or sensor noise were
	///enabled or disabled by user
	void noiseSettingsChanged();

	/// Emitted when user has started intepretation from the 2D model window.
	void runButtonPressed();

	/// Emitted when user has stopped intepretation from the 2D model window.
	void stopButtonPressed();

protected:
	virtual void changeEvent(QEvent *e);
	virtual void showEvent(QShowEvent *e);
	virtual void keyPressEvent(QKeyEvent *event);

	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, const interpreterBase::robotModel::DeviceInfo &device) override;

private slots:
	void bringToFront();

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

	void alignWalls();
	void changePhysicsSettings();

	void toggleDisplayVisibility();

private:
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

	enum CursorType
	{
		noDrag = 0
		, hand
		, multiselection
		, drawWall
		, drawLine
		, drawStylus
		, drawEllipse
	};

	static const int defaultPenWidth = 15;

	static const int indexOfNoneSensor = 0;
	static const int indexOfTouchSensor = 1;
	static const int indexOfColorSensor = 2;
	static const int indexOfSonarSensor = 3;
	static const int indexOfLightSensor = 4;

	struct RobotState {
	public:
		RobotState();

		QPointF pos;
		double rotation;
	};

	void connectUiButtons();
	void initButtonGroups();
	void initPorts();
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
	void setCursorTypeForDrawing(CursorType type);
	void setCursorType(CursorType cursor);

	void initWidget();
	QList<graphicsUtils::AbstractItem *> selectedColorItems();
	bool isColorItem(graphicsUtils::AbstractItem *item);

	void centerOnRobot();
	QGraphicsView::DragMode cursorTypeToDragType(CursorType type) const;
	QCursor cursorTypeToCursor(CursorType type) const;
	void processDragMode();
	void syncCursorButtons();

	void onFirstShow();

	void initRunStopButtons();

	void updateWheelComboBoxes();

	Ui::D2Form *mUi;
	D2ModelScene *mScene;
	RobotItem *mRobot;

	model::Model &mModel;

	engine::TwoDModelDisplayWidget *mDisplay;

	/// Current action (toggled button on left panel)
	DrawingAction mDrawingAction;

	/// Variable to count clicks on scene, used to create walls
	int mMouseClicksCount;

	/// Temporary wall that's being created. When it's complete, it's added to world model
	items::WallItem *mCurrentWall;
	items::LineItem *mCurrentLine;
	items::StylusItem *mCurrentStylus;
	items::EllipseItem *mCurrentEllipse;

	int mWidth;

	bool mClearing;

	QButtonGroup mButtonGroup;
	QButtonGroup mCursorButtonGroup;

	CursorType mNoneCursorType; // cursorType for noneStatus
	CursorType mCursorType; // current cursorType

	bool mFollowRobot;

	bool mFirstShow;

	RobotState mInitialRobotBeforeRun;

	QScopedPointer<Configurer const> mConfigurer;
};

}
}
