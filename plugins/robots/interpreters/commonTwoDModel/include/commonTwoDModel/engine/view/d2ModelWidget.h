#pragma once

#include <QtCore/QSignalMapper>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>

#include <qrutils/qRealDialog.h>
#include <qrutils/graphicsUtils/lineImpl.h>

#include <interpreterBase/devicesConfigurationWidget.h>
#include <interpreterBase/devicesConfigurationProvider.h>

#include "commonTwoDModel/engine/twoDModelDisplayWidget.h"

class QComboBox;
class QDomDocument;

namespace Ui {
class D2Form;
}

namespace graphicsUtils {
class AbstractItem;
}

namespace twoDModel {

namespace model {
class Model;
class RobotModel;
}

namespace view {

class D2ModelScene;
class SensorItem;
class RobotItem;

class D2ModelWidget : public utils::QRealDialog, public interpreterBase::DevicesConfigurationProvider
{
	Q_OBJECT

public:
	D2ModelWidget(model::Model &model, QWidget *parent = 0);
	~D2ModelWidget();

	void init();
	void close();

	/// If auto-open is enabled 2D model window will appear each time when interpreter run action triggered.
	/// By default this option is enabled.
	void setAutoOpen(bool enabled);

	D2ModelScene *scene();
	engine::TwoDModelDisplayWidget *display();

	SensorItem *sensorItem(interpreterBase::robotModel::PortInfo const &port);
	void setSensorVisible(interpreterBase::robotModel::PortInfo const &port, bool isVisible);

	void loadXml(QDomDocument const &worldModel);

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
	void changeEvent(QEvent *e) override;
	void showEvent(QShowEvent *e) override;
	void keyPressEvent(QKeyEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

	void onDeviceConfigurationChanged(QString const &robotModel
			, interpreterBase::robotModel::PortInfo const &port
			, const interpreterBase::robotModel::DeviceInfo &device
			, Reason reason) override;

private slots:
	void bringToFront();

	void saveToRepo();
	void saveWorldModel();
	void loadWorldModel();

	void changePenWidth(int width);
	void changePenColor(int textIndex);
	void onSelectionChange();

	void changeSpeed(int curIndex);

	void enableRobotFollowing(bool on);
	void onHandCursorButtonToggled(bool on);
	void onMultiselectionCursorButtonToggled(bool on);

	void changePhysicsSettings();

	void toggleDisplayVisibility();

	void saveInitialRobotBeforeRun();
	void setInitialRobotBeforeRun();

private:
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

	static const int defaultPenWidth = 6;

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

	void changePalette();
	void connectUiButtons();
	void initButtonGroups();
	void setPortsGroupBoxAndWheelComboBoxes();
	void unsetPortsGroupBoxAndWheelComboBoxes();

	void setHighlightOneButton(QAbstractButton * const oneButton);

	void setDisplayVisibility(bool visible);

	QDomDocument generateXml() const;

	/// Set active panel toggle button and deactivate all others
	void setActiveButton(int active);

	/// Get QPushButton for current sensor
	QPushButton *currentPortButton();

	/// Reread sensor configuration on given port, delete old sensor item and create new.
	void reinitSensor(RobotItem *robotItem, interpreterBase::robotModel::PortInfo const &port);

	void setValuePenColorComboBox(QColor const &penColor);
	void setValuePenWidthSpinBox(int width);
	void setItemPalette(QPen const &penItem, QBrush const &brushItem);

	void setCursorTypeForDrawing(CursorType type);
	void setCursorType(CursorType cursor);

	void initWidget();
	QList<graphicsUtils::AbstractItem *> selectedColorItems() const;
	bool isColorItem(graphicsUtils::AbstractItem * const item) const;

	void centerOnRobot();
	QGraphicsView::DragMode cursorTypeToDragType(CursorType type) const;
	QCursor cursorTypeToCursor(CursorType type) const;
	void refreshCursor();
	void syncCursorButtons();

	void onFirstShow();

	void initRunStopButtons();

	void updateWheelComboBoxes();

	void onRobotListChange(RobotItem *robotItem);

	void setSelectedRobotItem(RobotItem *robotItem);
	void unsetSelectedRobotItem();

	Ui::D2Form *mUi = nullptr;
	D2ModelScene *mScene = nullptr;

	RobotItem *mSelectedRobotItem = nullptr;
	interpreterBase::DevicesConfigurationWidget *mCurrentConfigurer;

	model::Model &mModel;

	engine::TwoDModelDisplayWidget *mDisplay = nullptr;

	int mWidth = 0;

	QButtonGroup mButtonGroup;
	QButtonGroup mCursorButtonGroup;

	CursorType mNoneCursorType; // cursorType for noneStatus
	CursorType mCursorType; // current cursorType

	bool mFollowRobot = false;

	bool mFirstShow = true;

	QMap<model::RobotModel *, RobotState> mInitialRobotsBeforeRun;

	bool mDisplayIsVisible = false;

	bool mAutoOpen;
};

}
}
