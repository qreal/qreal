/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtCore/QSignalMapper>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>

#include <qrutils/graphicsUtils/lineImpl.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/editorInterface.h>
#include <kitBase/readOnly.h>

#include <kitBase/devicesConfigurationWidget.h>
#include <kitBase/devicesConfigurationProvider.h>

#include "twoDModel/engine/twoDModelDisplayWidget.h"

#include "twoDModel/twoDModelDeclSpec.h"

class QComboBox;
class QPushButton;
class QDomDocument;

namespace Ui {
class TwoDModelWidget;
}

namespace graphicsUtils {
class AbstractItem;
}

namespace qReal {
class ControllerInterface;
}

namespace twoDModel {

namespace model {
class Model;
class RobotModel;
}

namespace view {

class TwoDModelScene;
class SensorItem;
class RobotItem;
class ActionsBox;
class ColorItemPopup;
class ImageItemPopup;
class RobotItemPopup;
class SpeedPopup;

class TWO_D_MODEL_EXPORT TwoDModelWidget : public QWidget
		, public kitBase::DevicesConfigurationProvider
		, public qReal::EditorInterface
{
	Q_OBJECT

public:
	TwoDModelWidget(model::Model &model, QWidget *parent = 0);
	~TwoDModelWidget();

	/// Overrides default closing behaviour with just hiding window.
	void close();

	TwoDModelScene *scene();
	engine::TwoDModelDisplayWidget *display();

	SensorItem *sensorItem(const kitBase::robotModel::PortInfo &port);
	void setSensorVisible(const kitBase::robotModel::PortInfo &port, bool isVisible);

	void loadXmls(const QDomDocument &worldModel, const QDomDocument &blobs);

	/// Returns a reference to a model part of 2D model MVC architecture.
	model::Model &model() const;

	/// Passes into view a reference to controller object that will execute commands.
	void setController(qReal::ControllerInterface &controller);

	/// Sets groups of items on 2d model that can not be modified by user. Used for "challenge" mode where student
	/// shall provide program that makes robot do specific task in given unmodifyable world model.
	/// @see ReadOnly
	void setInteractivityFlags(kitBase::ReadOnlyFlags flags);

	/// Enables or disables compact 2D model mode.
	/// In a compact mode 2D model window has less controls, they may seem in another way.
	void setCompactMode(bool enabled);

	QString editorId() const override;
	bool supportsZooming() const override;
	void configure(QAction &zoomIn, QAction &zoomOut, QAction &undo, QAction &redo
		, QAction &copy, QAction &paste, QAction &cut, QAction &find) override;

public slots:
	void zoomIn() override;
	void zoomOut() override;

signals:
	/// Emitted each time when user closes 2D model window.
	void widgetClosed();

	void robotWasIntersectedByWall(bool isNeedStop, const QPointF &oldPos);

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

	void focusInEvent(QFocusEvent *event) override;

	void onDeviceConfigurationChanged(const QString &robotId
			, const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &device
			, Reason reason) override;

private slots:
	void bringToFront();

	void saveWorldModelToRepo();
	void saveBlobsToRepo();

	void saveWorldModel();
	void loadWorldModel();
	void setBackground();

	void onSelectionChange();

	void speedUp();
	void speedDown();
	void checkSpeedButtons();

	void enableRobotFollowing(bool on);
	void onHandCursorActionTriggered();
	void onMultiselectionCursorActionTriggered();

	void changePhysicsSettings();

	void toggleDetailsVisibility();

	void returnToStartMarker();

	void trainingModeChanged(bool enabled);

private:
	enum CursorType
	{
		noDrag = 0
		, hand
		, multiselection
		, drawWall
		, drawSkittle
		, drawBall
		, drawLine
		, drawStylus
		, drawEllipse
	};

	static const int defaultPenWidth = 6;

	void connectUiButtons();
	void setPortsGroupBoxAndWheelComboBoxes();
	void unsetPortsGroupBoxAndWheelComboBoxes();

	void setDetailsVisibility(bool visible);
	void setRunStopButtonsVisibility();

	QDomDocument generateWordModelXml() const;
	QDomDocument generateBlobsXml() const;
	QDomDocument generateWordModelWithBlobsXml() const;

	/// Set active panel toggle button and deactivate all others
	void setActiveButton(int active);

	/// Get QPushButton for current sensor
	QPushButton *currentPortButton();

	void setCursorTypeForDrawing(CursorType type);
	void setCursorType(int cursorType);
	void setCursorType(CursorType cursor);

	void initWidget();
	void initPalette();
	void initDetailsTab();

	QList<graphicsUtils::AbstractItem *> selectedColorItems() const;
	bool isColorItem(graphicsUtils::AbstractItem * const item) const;

	void centerOnRobot();
	QGraphicsView::DragMode cursorTypeToDragType(CursorType type) const;
	QCursor cursorTypeToCursor(CursorType type) const;
	void refreshCursor();
	void syncCursorButtons();

	void onFirstShow();

	void initRunStopButtons();

	bool setSelectedPort(QComboBox * const comboBox, const kitBase::robotModel::PortInfo &port);
	void updateWheelComboBoxes();

	void onRobotListChange(RobotItem *robotItem);

	void setSelectedRobotItem(RobotItem *robotItem);
	void unsetSelectedRobotItem();

	void incrementTimelineCounter();

	Ui::TwoDModelWidget *mUi = nullptr;
	TwoDModelScene *mScene = nullptr;
	QScopedPointer<ActionsBox> mActions;
	ColorItemPopup *mColorFieldItemPopup;  // Takes ownership
	ImageItemPopup *mImageItemPopup;  // Takes ownership
	RobotItemPopup *mRobotItemPopup;  // Takes ownership
	SpeedPopup *mSpeedPopup;  // Takes owneship

	RobotItem *mSelectedRobotItem = nullptr;
	kitBase::DevicesConfigurationWidget *mCurrentConfigurer;

	model::Model &mModel;
	qReal::ControllerInterface *mController = nullptr;

	engine::TwoDModelDisplayWidget *mDisplay = nullptr;
	engine::TwoDModelDisplayWidget *mNullDisplay = nullptr;

	int mCurrentSpeed;

	CursorType mNoneCursorType; // cursorType for noneStatus
	CursorType mCursorType; // current cursorType

	bool mFollowRobot = false;
	bool mDetailsAreVisible = false;
	bool mFirstShow = true;

	bool mSensorsReadOnly = false;
	bool mRobotPositionReadOnly = false;

	bool mCompactMode = false;
};

}
}
