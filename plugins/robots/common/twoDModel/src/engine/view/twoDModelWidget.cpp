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

#include "twoDModel/engine/view/twoDModelWidget.h"
#include "ui_twoDModelWidget.h"

#include <QtCore/qmath.h>
#include <QtCore/QDebug>
#include <QtWidgets/QMessageBox>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/qRealFileDialog.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <kitBase/devicesConfigurationWidget.h>
#include <kitBase/robotModel/robotParts/motor.h>
#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/colorSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <kitBase/robotModel/robotParts/vectorSensor.h>

#include "parts/actionsBox.h"

#include "scene/sensorItem.h"
#include "scene/sonarSensorItem.h"
#include "scene/twoDModelScene.h"
#include "scene/robotItem.h"

#include "src/engine/items/wallItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/stylusItem.h"

#include "twoDModel/engine/model/constants.h"
#include "twoDModel/engine/model/model.h"

#include "nullTwoDModelDisplayWidget.h"

using namespace twoDModel;
using namespace view;
using namespace model;
using namespace qReal;
using namespace utils;
using namespace graphicsUtils;
using namespace kitBase;
using namespace kitBase::robotModel;
using namespace robotParts;

TwoDModelWidget::TwoDModelWidget(Model &model, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::TwoDModelWidget)
	, mActions(new ActionsBox)
	, mModel(model)
	, mDisplay(new twoDModel::engine::NullTwoDModelDisplayWidget())
	, mWidth(defaultPenWidth)
{
	setWindowIcon(QIcon(":/icons/2d-model.svg"));

	initWidget();

	connectUiButtons();
	mUi->realisticPhysicsCheckBox->setChecked(mModel.settings().realisticPhysics());
	mUi->enableSensorNoiseCheckBox->setChecked(mModel.settings().realisticSensors());
	mUi->enableMotorNoiseCheckBox->setChecked(mModel.settings().realisticMotors());
	changePhysicsSettings();

	connect(mScene, &TwoDModelScene::selectionChanged, this, &TwoDModelWidget::onSelectionChange);
	connect(mScene, &TwoDModelScene::mousePressed, this, &TwoDModelWidget::refreshCursor);
	connect(mScene, &TwoDModelScene::mouseReleased, this, &TwoDModelWidget::refreshCursor);
	connect(mScene, &TwoDModelScene::mouseReleased, this, [this](){ saveToRepo(); });
	connect(mScene, &TwoDModelScene::robotPressed, [this]() { mUi->noneButton->setChecked(true); });
	connect(mScene, &TwoDModelScene::robotListChanged, this, &TwoDModelWidget::onRobotListChange);

	connect(&mModel.timeline(), &Timeline::started, [this]() { bringToFront(); mUi->timelineBox->setValue(0); });
	connect(&mModel.timeline(), &Timeline::tick, this, &TwoDModelWidget::incrementTimelineCounter);
	connect(&mModel.timeline(), &Timeline::started, this, &TwoDModelWidget::setRunStopButtonsVisibility);
	connect(&mModel.timeline(), &Timeline::stopped, this, &TwoDModelWidget::setRunStopButtonsVisibility);
	setRunStopButtonsVisibility();

	enableRobotFollowing(SettingsManager::value("2dFollowingRobot").toBool());
//	mUi->autoCenteringButton->setChecked(mFollowRobot);
	mUi->noneButton->setChecked(true);

	setFocus();

	mUi->timelineBox->setSingleStep(Timeline::timeInterval * 0.001);
}

TwoDModelWidget::~TwoDModelWidget()
{
	mSelectedRobotItem = nullptr;
	delete mScene;
	delete mDisplay;
	delete mUi;
}

void TwoDModelWidget::initWidget()
{
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

	mUi->setupUi(this);

	mToolsTabName = mUi->mainTabBar->tabText(0);
	mPortsTabName = mUi->mainTabBar->tabText(1);
	mModelSettingsTabName = mUi->mainTabBar->tabText(2);

	mScene = new TwoDModelScene(mModel, mUi->graphicsView);
	connectDevicesConfigurationProvider(mScene);
	mScene->addActions(mActions->sceneContextMenuActions());
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	move(0, 0);

//	mUi->penWidthSpinBox->setRange(1, 30);
//	mUi->penWidthSpinBox->setValue(mWidth);
//	mUi->penColorComboBox->setColor(QColor("black"));

//	const QStringList colorList = { "Black", "Blue", "Green", "Yellow", "Red" };
//	const QStringList translatedColorList = { tr("Black"), tr("Blue"), tr("Green"), tr("Yellow"), tr("Red") };

//	mUi->penColorComboBox->setColorList(colorList, translatedColorList);
//	mUi->penColorComboBox->setColor(QColor("black"));

	initButtonGroups();

	mDisplay->setMinimumSize(displaySize);
	mDisplay->setMaximumSize(displaySize);
	static_cast<QHBoxLayout *>(mUi->displayFrame->layout())->insertWidget(0, mDisplay);
	mUi->displayFrame->setEnabled(false);

	setDisplayVisibility(SettingsManager::value("2d_displayVisible").toBool());

	connect(mUi->gridParametersBox, SIGNAL(parametersChanged()), mScene, SLOT(update()));
	connect(mUi->gridParametersBox, &GridParameters::parametersChanged, mScene, &TwoDModelScene::alignWalls);
}

void TwoDModelWidget::connectUiButtons()
{
	connect(mUi->realisticPhysicsCheckBox, &QAbstractButton::toggled, this, &TwoDModelWidget::changePhysicsSettings);
	connect(mUi->enableMotorNoiseCheckBox, &QAbstractButton::toggled, this, &TwoDModelWidget::changePhysicsSettings);
	connect(mUi->enableSensorNoiseCheckBox, &QAbstractButton::toggled, this, &TwoDModelWidget::changePhysicsSettings);

	connect(mUi->ellipseButton, &QAbstractButton::toggled, mScene, &TwoDModelScene::addEllipse);
	connect(mUi->stylusButton, &QAbstractButton::toggled, mScene, &TwoDModelScene::addStylus);
	connect(mUi->lineButton, &QAbstractButton::toggled, mScene, &TwoDModelScene::addLine);
	connect(mUi->wallButton, &QAbstractButton::toggled, mScene, &TwoDModelScene::addWall);
	connect(mUi->noneButton, &QAbstractButton::toggled, mScene, &TwoDModelScene::setNoneStatus);

	connect(mUi->ellipseButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawEllipse); });
	connect(mUi->stylusButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawStylus); });
	connect(mUi->lineButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawLine); });
	connect(mUi->wallButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawWall); });
	connect(mUi->noneButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(mNoneCursorType); });

	connect(mUi->clearButton, &QAbstractButton::clicked, [this](){
		QMessageBox confirmation;
		confirmation.setWindowTitle(tr("Warning"));
		confirmation.setText(tr("Do you really want to clear scene?"));
		confirmation.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
		confirmation.setButtonText(QMessageBox::Yes, tr("Yes"));
		confirmation.setButtonText(QMessageBox::Cancel, tr("Cancel"));
		if (QMessageBox::Yes == confirmation.exec()) {
			mScene->clearScene(false, Reason::userAction);
		}
	});

	connect(&mActions->clearFloorAction(), &QAction::triggered, &mModel.worldModel(), &WorldModel::clearRobotTrace);
	connect(&mModel.worldModel(), &WorldModel::robotTraceAppearedOrDisappeared
			, &mActions->clearFloorAction(), &QAction::setVisible, Qt::QueuedConnection);

	connect(&mButtonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled)
			, [this](QAbstractButton *button, bool toggled) {
				if (toggled) {
					setHighlightOneButton(button);
				} else if (!mButtonGroup.checkedButton()) {
					setHighlightOneButton(mUi->noneButton);
				}
	});

//	connect(mUi->penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
//	connect(mUi->penColorComboBox, SIGNAL(activated(int)), this, SiLOT(changePenColor(int)));

	connect(&mActions->saveModelAction(), &QAction::triggered, this, &TwoDModelWidget::saveWorldModel);
	connect(&mActions->loadModelAction(), &QAction::triggered, this, &TwoDModelWidget::loadWorldModel);

//	connect(mUi->speedComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSpeed(int)));

//	connect(mUi->autoCenteringButton, SIGNAL(toggled(bool)), this, SLOT(enableRobotFollowing(bool)));
	connect(&mActions->scrollHandModeAction(), &QAction::toggled
			, this, &TwoDModelWidget::onHandCursorButtonToggled);
	connect(&mActions->multiSelectionModeAction(), &QAction::toggled
			, this, &TwoDModelWidget::onMultiselectionCursorButtonToggled);

//	connect(mUi->initialStateButton, SIGNAL(clicked()), this, SLOT(returnToStartMarker()));
	connect(mUi->displayButton, SIGNAL(clicked()), this, SLOT(toggleDisplayVisibility()));

	initRunStopButtons();
}

void TwoDModelWidget::initButtonGroups()
{
	mButtonGroup.setExclusive(false);
	mButtonGroup.addButton(mUi->lineButton);
	mButtonGroup.addButton(mUi->wallButton);
	mButtonGroup.addButton(mUi->stylusButton);
	mButtonGroup.addButton(mUi->ellipseButton);
	mButtonGroup.addButton(mUi->noneButton);
}

void TwoDModelWidget::setPortsGroupBoxAndWheelComboBoxes()
{
	mCurrentConfigurer = new DevicesConfigurationWidget(mUi->portsGroupBox, true, true);
	mCurrentConfigurer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	mCurrentConfigurer->loadRobotModels({ &mSelectedRobotItem->robotModel().info() });
	mCurrentConfigurer->selectRobotModel(mSelectedRobotItem->robotModel().info());
	mUi->portsGroupBox->layout()->addWidget(mCurrentConfigurer);
	mCurrentConfigurer->connectDevicesConfigurationProvider(&mSelectedRobotItem->robotModel().configuration());
	connectDevicesConfigurationProvider(&mSelectedRobotItem->robotModel().configuration());

	auto connectWheelComboBox = [this](QComboBox * const comboBox, RobotModel::WheelEnum wheel) {
				connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
						, [this, wheel, comboBox](int index) {
								mSelectedRobotItem->robotModel().setMotorPortOnWheel(wheel
										, comboBox->itemData(index).value<PortInfo>());
						});
		};

	connectWheelComboBox(mUi->leftWheelComboBox, RobotModel::left);
	connectWheelComboBox(mUi->rightWheelComboBox, RobotModel::right);
}

void TwoDModelWidget::unsetPortsGroupBoxAndWheelComboBoxes()
{
	if (mCurrentConfigurer) {
		mUi->portsGroupBox->layout()->removeWidget(mCurrentConfigurer);
		mCurrentConfigurer->disconnectDevicesConfigurationProvider();

		if (mSelectedRobotItem) {
			mSelectedRobotItem->robotModel().configuration().disconnectDevicesConfigurationProvider();
		}

		delete mCurrentConfigurer;
	}
}

void TwoDModelWidget::setHighlightOneButton(QAbstractButton * const oneButton)
{
	for (QAbstractButton * const button : mButtonGroup.buttons()) {
		if (button != oneButton && button->isChecked()) {
			// This will disable toggled(false) emission, we are interested only in checked buttons.
			button->blockSignals(true);
			button->setChecked(false);
			button->blockSignals(false);
		}
	}

	if (!oneButton->isChecked()) {
		oneButton->setChecked(true);
	}
}

void TwoDModelWidget::changeSpeed(int curIndex)
{
	switch(curIndex){
	case 0:
		mModel.timeline().setSpeedFactor(Timeline::slowSpeedFactor);
		break;
	case 1:
		mModel.timeline().setSpeedFactor(Timeline::normalSpeedFactor);
		break;
	case 2:
		mModel.timeline().setSpeedFactor(Timeline::fastSpeedFactor);
		break;
	default:
		mModel.timeline().setSpeedFactor(Timeline::normalSpeedFactor);
	}
}

void TwoDModelWidget::returnToStartMarker()
{
	for (RobotModel * const model : mModel.robotModels()) {
		mScene->robot(*model)->returnToStartPosition();
	}
}

void TwoDModelWidget::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);
	if ((event->key() == Qt::Key_Equal || event->key() == Qt::Key_Plus) && event->modifiers() == Qt::ControlModifier) {
		mScene->mainView()->zoomIn();
	} else if (event->matches(QKeySequence::ZoomOut)) {
		mScene->mainView()->zoomOut();
	} else if (event->key() == Qt::Key_F5) {
		mUi->runButton->animateClick();
	} else if (event->key() == Qt::Key_Escape) {
		mUi->stopButton->animateClick();
	}
}

void TwoDModelWidget::close()
{
	setVisible(false);
}

void TwoDModelWidget::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		mUi->retranslateUi(this);
		break;
	default:
		break;
	}
}

void TwoDModelWidget::showEvent(QShowEvent *e)
{
	e->accept();
	QWidget::showEvent(e);
	if (mFirstShow) {
		mFirstShow = false;
		onFirstShow();
	}
}

void TwoDModelWidget::onFirstShow()
{
	mUi->speedComboBox->setCurrentIndex(1); // Normal speed
	setCursorType(static_cast<CursorType>(SettingsManager::value("2dCursorType").toInt()));
}

void TwoDModelWidget::centerOnRobot()
{
	if (mSelectedRobotItem && mFollowRobot && mSelectedRobotItem->robotModel().onTheGround()) {
		mScene->centerOnRobot(mSelectedRobotItem);
	}
}

void TwoDModelWidget::setCursorTypeForDrawing(CursorType type)
{
	mCursorType = type;
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));
}

void TwoDModelWidget::saveWorldModel()
{
	// Saves world and robot models simultaneously, for now.
	QString saveFileName = QRealFileDialog::getSaveFileName("Save2DModelWidget", this
			, tr("Saving world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (saveFileName.isEmpty()) {
		return;
	}

	if (!saveFileName.toLower().endsWith(".xml")) {
		saveFileName += ".xml";
	}

	const QDomDocument save = generateXml();

	utils::OutFile saveFile(saveFileName);
	saveFile() << "<?xml version='1.0' encoding='utf-8'?>\n";
	saveFile() << save.toString(4);
}

void TwoDModelWidget::loadWorldModel()
{
	// Loads world and robot models simultaneously.
	const QString loadFileName = QRealFileDialog::getOpenFileName("Open2DModelWidget", this
			, tr("Loading world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (loadFileName.isEmpty()) {
		return;
	}

	QString errorMessage;
	int errorLine = 0;
	int errorColumn = 0;
	const QDomDocument save = utils::xmlUtils::loadDocument(loadFileName, &errorMessage, &errorLine, &errorColumn);
	if (!errorMessage.isEmpty()) {
		mModel.errorReporter()->addError(QString("%1:%2: %3")
				.arg(QString::number(errorLine), QString::number(errorColumn), errorMessage));
	}

	loadXml(save);
}

void TwoDModelWidget::reinitSensor(RobotItem *robotItem, const PortInfo &port)
{
	robotItem->removeSensor(port);
	RobotModel &robotModel = robotItem->robotModel();

	const DeviceInfo &device = robotModel.configuration().type(port);
	if (device.isNull() || (
			/// @todo: Add supported by 2D model sensors here
			!device.isA<TouchSensor>()
			&& !device.isA<ColorSensor>()
			&& !device.isA<LightSensor>()
			&& !device.isA<RangeSensor>()
			/// @todo For working with line sensor from TRIK. Actually this information shall be loaded from plugins.
			&& !device.isA<VectorSensor>()
			))
	{
		return;
	}

	SensorItem *sensor = device.isA<RangeSensor>()
			? new SonarSensorItem(mModel.worldModel(), robotModel.configuration()
					, port
					, robotModel.info().sensorImagePath(device)
					, robotModel.info().sensorImageRect(device)
					)
			: new SensorItem(robotModel.configuration()
					, port
					, robotModel.info().sensorImagePath(device)
					, robotModel.info().sensorImageRect(device)
					);

	sensor->setEditable(!mSensorsReadOnly);

	robotItem->addSensor(port, sensor);
}

bool TwoDModelWidget::isColorItem(AbstractItem * const item) const
{
	return dynamic_cast<items::ColorFieldItem *>(item)
			&& !dynamic_cast<items::WallItem *>(item);
}

QList<AbstractItem *> TwoDModelWidget::selectedColorItems() const
{
	QList<AbstractItem *> resList;
	for (QGraphicsItem * const graphicsItem : mScene->selectedItems()) {
		AbstractItem *item = dynamic_cast<AbstractItem*>(graphicsItem);
		if (item && (isColorItem(item) || dynamic_cast<RobotItem *>(item))) {
			resList << item;
		}
	}

	return resList;
}

void TwoDModelWidget::changePenWidth(int width)
{
	mScene->setPenWidthItems(width);
	mWidth = width;
	for (AbstractItem * const item : selectedColorItems()) {
		item->setPenWidth(width);
	}

	mScene->update();
}

void TwoDModelWidget::changePenColor(int textIndex)
{
//	const QString text = mUi->penColorComboBox->colorByIndex(textIndex).name();
//	mScene->setPenColorItems(text);
//	for (AbstractItem * const item : selectedColorItems()) {
//		item->setPenColor(text);
//	}

	mScene->update();
}

void TwoDModelWidget::changePalette()
{
	const QList<QGraphicsItem *> listSelectedItems = mScene->selectedItems();
	if (!listSelectedItems.isEmpty()) {
		AbstractItem *item = dynamic_cast<AbstractItem *>(listSelectedItems.back());
		if (isColorItem(item)) {
			const QPen penItem = item->pen();
			const QBrush brushItem = item->brush();
			setItemPalette(penItem, brushItem);
			mScene->setPenBrushItems(penItem, brushItem);
		}
	}
}

void TwoDModelWidget::onSelectionChange()
{
	changePalette();

	if (mScene->oneRobot()) {
		return;
	}

	const QList<QGraphicsItem *> listSelectedItems = mScene->selectedItems();
	RobotItem *robotItem = nullptr;
	bool oneRobotItem = false;

	for (QGraphicsItem *item : listSelectedItems) {
		if (dynamic_cast<RobotItem *>(item)) {
			robotItem = dynamic_cast<RobotItem *>(item);
			if (oneRobotItem) {
				if (mSelectedRobotItem) {
					unsetSelectedRobotItem();
				}

				return;
			}
			oneRobotItem = true;
		}
	}

	if (oneRobotItem
			&& mSelectedRobotItem
			&& robotItem->robotModel().info().robotId() == mSelectedRobotItem->robotModel().info().robotId())
	{
		return;
	}

	if (mSelectedRobotItem) {
		unsetSelectedRobotItem();
	}

	if (oneRobotItem) {
		if (robotItem->robotModel().info().name() == "NullTwoDRobotModel") {
			return;
		}

		setSelectedRobotItem(robotItem);
	}
}

void TwoDModelWidget::setValuePenColorComboBox(const QColor &penColor)
{
//	mUi->penColorComboBox->setColor(penColor);
}

void TwoDModelWidget::setValuePenWidthSpinBox(int width)
{
//	mUi->penWidthSpinBox->setValue(width);
}

void TwoDModelWidget::setItemPalette(const QPen &penItem, const QBrush &brushItem)
{
	Q_UNUSED(brushItem)
//	mUi->penColorComboBox->blockSignals(true);
//	mUi->penWidthSpinBox->blockSignals(true);
//	setValuePenWidthSpinBox(penItem.width());
//	setValuePenColorComboBox(penItem.color());
//	mUi->penColorComboBox->blockSignals(false);
//	mUi->penWidthSpinBox->blockSignals(false);
}

TwoDModelScene *TwoDModelWidget::scene()
{
	return mScene;
}

engine::TwoDModelDisplayWidget *TwoDModelWidget::display()
{
	return mDisplay;
}

void TwoDModelWidget::setSensorVisible(const kitBase::robotModel::PortInfo &port, bool isVisible)
{
	RobotModel *robotModel = mModel.robotModels()[0];

	if (mScene->robot(*robotModel)->sensors()[port]) {
		mScene->robot(*robotModel)->sensors()[port]->setVisible(isVisible);
	}
}

void TwoDModelWidget::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event)
	emit widgetClosed();
}

SensorItem *TwoDModelWidget::sensorItem(const kitBase::robotModel::PortInfo &port)
{
	return mScene->robot(*mModel.robotModels()[0])->sensors().value(port);
}

void TwoDModelWidget::saveToRepo()
{
	emit mModel.modelChanged(generateXml());
}

QDomDocument TwoDModelWidget::generateXml() const
{
	return mModel.serialize();
}

void TwoDModelWidget::loadXml(const QDomDocument &worldModel)
{
	mScene->clearScene(true, Reason::loading);
	mModel.deserialize(worldModel);
}

Model &TwoDModelWidget::model() const
{
	return mModel;
}

void TwoDModelWidget::setInteractivityFlags(ReadOnlyFlags flags)
{
	const auto openTab = [this](QWidget * const tab) {
		QList<const QWidget *> tabOrder{mUi->toolsTab, mUi->portsTab, mUi->modelSettingsTab};
		QHash<const QWidget *, QString> tabs{
				{mUi->toolsTab, mToolsTabName}
				, {mUi->portsTab, mPortsTabName}
				, {mUi->modelSettingsTab, mModelSettingsTabName}
		};

		if (!tabs.keys().contains(tab)) {
			throw qReal::Exception("Trying to add unknown tab to tools palette in 2d model");
		}

		int tabsIndex = 0;

		for (int i = 0; i < mUi->mainTabBar->count(); ++i) {
			if (mUi->mainTabBar->widget(i) == tab) {
				return;
			} else {
				while (tabsIndex < tabOrder.size() && mUi->mainTabBar->widget(i) != tabOrder[tabsIndex]) {
					if (tabOrder[tabsIndex] == tab) {
						mUi->mainTabBar->insertTab(i, tab, tabs[tab]);
						return;
					}

					++tabsIndex;
				}
			}
		}

		mUi->mainTabBar->insertTab(mUi->mainTabBar->count(), tab, tabs[tab]);
	};

	const auto closeTab = [this](QWidget * const tab) {
		for (int i = 0; i < mUi->mainTabBar->count(); ++i) {
			if (mUi->mainTabBar->widget(i) == tab) {
				mUi->mainTabBar->removeTab(i);
				--i;
			}
		}
	};

	const auto setTabHidden = [this, &openTab, &closeTab](QWidget * const tab, const bool hidden) {
		if (hidden) {
			closeTab(tab);
		} else {
			openTab(tab);
		}
	};

	const bool worldReadOnly = (flags & ReadOnly::World) != 0;

	setTabHidden(mUi->toolsTab, worldReadOnly);

	const auto hasSpacer = [this]() {
		for (int i = 0; i < mUi->sceneHeaderWidget->layout()->count(); ++i) {
			if (mUi->sceneHeaderWidget->layout()->itemAt(i) == mUi->horizontalSpacer) {
				return true;
			}
		}

		return false;
	};

	mUi->gridParametersBox->setVisible(!worldReadOnly);
	if (!worldReadOnly && hasSpacer()) {
		mUi->sceneHeaderWidget->layout()->removeItem(mUi->horizontalSpacer);
	} else if (worldReadOnly && !hasSpacer()){
		static_cast<QHBoxLayout *>(mUi->sceneHeaderWidget->layout())->insertItem(1, mUi->horizontalSpacer);
	}

	mActions->saveModelAction().setVisible(!worldReadOnly);
	mActions->loadModelAction().setVisible(!worldReadOnly);

	const bool sensorsReadOnly = flags.testFlag(ReadOnly::Sensors);
	const bool robotConfigurationReadOnly = flags.testFlag(ReadOnly::RobotSetup);

	setTabHidden(mUi->portsTab, sensorsReadOnly && robotConfigurationReadOnly);

	mCurrentConfigurer->setEnabled(!sensorsReadOnly);
	mUi->leftWheelComboBox->setEnabled(!robotConfigurationReadOnly);
	mUi->rightWheelComboBox->setEnabled(!robotConfigurationReadOnly);

	const bool simulationSettingsReadOnly = flags.testFlag(ReadOnly::SimulationSettings);

	mUi->realisticPhysicsCheckBox->setEnabled(!simulationSettingsReadOnly);
	mUi->enableMotorNoiseCheckBox->setEnabled(!simulationSettingsReadOnly);
	mUi->enableSensorNoiseCheckBox->setEnabled(!simulationSettingsReadOnly);

	mSensorsReadOnly = sensorsReadOnly;

	mScene->setInteractivityFlags(flags);
}

void TwoDModelWidget::setCompactMode(bool enabled)
{
	mCompactMode = enabled;
	setRunStopButtonsVisibility();
	mActions->setSaveLoadActionsShortcutsEnabled(!mCompactMode);
}

void TwoDModelWidget::enableRobotFollowing(bool on)
{
	mFollowRobot = on;
	SettingsManager::setValue("2dFollowingRobot", on);
}

void TwoDModelWidget::setCursorType(CursorType cursor)
{
	mNoneCursorType = cursor;
	mCursorType = mNoneCursorType;
	SettingsManager::setValue("2dCursorType", cursor);
	mUi->graphicsView->setDragMode(cursorTypeToDragType(cursor));
	mUi->graphicsView->setCursor(cursorTypeToCursor(cursor));
	syncCursorButtons();
}

void TwoDModelWidget::changePhysicsSettings()
{
	SettingsManager::setValue("2DModelRealisticPhysics", mUi->realisticPhysicsCheckBox->isChecked());
	SettingsManager::setValue("enableNoiseOfSensors", mUi->enableSensorNoiseCheckBox->isChecked());
	SettingsManager::setValue("enableNoiseOfMotors", mUi->enableMotorNoiseCheckBox->isChecked());

	mModel.settings().rereadNoiseSettings();
}

void TwoDModelWidget::toggleDisplayVisibility()
{
	setDisplayVisibility(!mDisplayIsVisible);
}

void TwoDModelWidget::setDisplayVisibility(bool visible)
{
	mDisplayIsVisible = visible;
	mDisplay->setVisible(visible);
	const QString direction = visible ? "right" : "left";
	mUi->displayButton->setIcon(QIcon(QString(":/icons/2d_%1.png").arg(direction)));
	SettingsManager::setValue("2d_displayVisible", visible);
}

void TwoDModelWidget::setRunStopButtonsVisibility()
{
	mUi->runButton->setVisible(!mCompactMode && !mModel.timeline().isStarted());
	mUi->stopButton->setVisible(!mCompactMode && mModel.timeline().isStarted());
}

QGraphicsView::DragMode TwoDModelWidget::cursorTypeToDragType(CursorType type) const
{
	switch(type) {
	case noDrag:
	case drawEllipse:
	case drawLine:
	case drawStylus:
	case drawWall:
		return QGraphicsView::NoDrag;
	case hand:
		return QGraphicsView::ScrollHandDrag;
	case multiselection:
		return QGraphicsView::RubberBandDrag;
	default:
		return QGraphicsView::ScrollHandDrag;
	}
}

QCursor TwoDModelWidget::cursorTypeToCursor(CursorType type) const
{
	switch(type) {
	case noDrag:
		return QCursor(Qt::ArrowCursor);
	case hand:
		return QCursor(Qt::OpenHandCursor);
	case multiselection:
		return QCursor(Qt::ArrowCursor);
	case drawLine:
		return QCursor(QPixmap(":/icons/2d_drawLineCursor.png"), 0, 0);
	case drawWall:
		return QCursor(QPixmap(":/icons/2d_drawWallCursor.png"), 0, 0);
	case drawEllipse:
		return QCursor(QPixmap(":/icons/2d_drawEllipseCursor.png"), 0, 0);
	case drawStylus:
		return QCursor(QPixmap(":/icons/2d_drawStylusCursor.png"), 0, 0);
	default:
		return Qt::ArrowCursor;
	}
}

void TwoDModelWidget::refreshCursor()
{
	mUi->graphicsView->setDragMode(cursorTypeToDragType(mCursorType));
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));
}

void TwoDModelWidget::onHandCursorButtonToggled(bool on)
{
	if (on) {
		setCursorType(hand);
	}
}

void TwoDModelWidget::onMultiselectionCursorButtonToggled(bool on)
{
	if (on) {
		setCursorType(multiselection);
	}
}

void TwoDModelWidget::syncCursorButtons()
{
	switch(mNoneCursorType) {
	case hand:
		mActions->scrollHandModeAction().setChecked(true);
		break;
	case multiselection:
		mActions->multiSelectionModeAction().setChecked(true);
		break;
	default:
		break;
	}
}

void TwoDModelWidget::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &device, Reason reason)
{
	Q_UNUSED(port)
	Q_UNUSED(device)
	Q_UNUSED(reason)

	/// @todo Convert configuration between models or something?
	if (mSelectedRobotItem && robotModel == mSelectedRobotItem->robotModel().info().robotId()) {
		updateWheelComboBoxes();
	}
}

void TwoDModelWidget::bringToFront()
{
	if (isHidden()) {
		show();
	}

	if (!isActiveWindow()) {
		activateWindow();
	}
}

void TwoDModelWidget::initRunStopButtons()
{
	connect(mUi->runButton, &QPushButton::clicked, this, &TwoDModelWidget::runButtonPressed);
	connect(mUi->stopButton, &QPushButton::clicked, this, &TwoDModelWidget::stopButtonPressed);
}

void TwoDModelWidget::updateWheelComboBoxes()
{
	if (!mSelectedRobotItem) {
		mUi->leftWheelComboBox->hide();
		mUi->rightWheelComboBox->hide();
		return;
	}

	const PortInfo leftWheelOldPort = mUi->leftWheelComboBox->currentData().value<PortInfo>();
	const PortInfo rightWheelOldPort = mUi->rightWheelComboBox->currentData().value<PortInfo>();

	mUi->leftWheelComboBox->clear();
	mUi->rightWheelComboBox->clear();

	/// @todo More general way of specifying uninitialized values, or someone actually will name some port as "None".
	mUi->leftWheelComboBox->addItem(tr("No wheel"), QVariant::fromValue(PortInfo("None", output)));
	mUi->rightWheelComboBox->addItem(tr("No wheel"), QVariant::fromValue(PortInfo("None", output)));

	for (const PortInfo &port : mSelectedRobotItem->robotModel().info().availablePorts()) {
		for (const DeviceInfo &device : mSelectedRobotItem->robotModel().info().allowedDevices(port)) {
			if (device.isA<Motor>()) {
				const QString item = tr("%1 (port %2)").arg(device.friendlyName(), port.userFriendlyName());
				mUi->leftWheelComboBox->addItem(item, QVariant::fromValue(port));
				mUi->rightWheelComboBox->addItem(item, QVariant::fromValue(port));
			}
		}
	}

	auto setSelectedPort = [](QComboBox * const comboBox, const PortInfo &port) {
		for (int i = 0; i < comboBox->count(); ++i) {
			if (comboBox->itemData(i).value<PortInfo>() == port) {
				comboBox->setCurrentIndex(i);
				return true;
			}
		}

		return false;
	};

	if (!setSelectedPort(mUi->leftWheelComboBox, leftWheelOldPort)) {
		if (!setSelectedPort(mUi->leftWheelComboBox
				, mSelectedRobotItem->robotModel().info().defaultLeftWheelPort())) {
			qDebug() << "Incorrect defaultLeftWheelPort set in configurer:"
					<< mSelectedRobotItem->robotModel().info().defaultLeftWheelPort().toString();

			if (mUi->leftWheelComboBox->count() > 1) {
				mUi->leftWheelComboBox->setCurrentIndex(1);
			}
		}
	}

	if (!setSelectedPort(mUi->rightWheelComboBox, rightWheelOldPort)) {
		if (!setSelectedPort(mUi->rightWheelComboBox
				, mSelectedRobotItem->robotModel().info().defaultRightWheelPort())) {

			qDebug() << "Incorrect defaultRightWheelPort set in configurer:"
					<< mSelectedRobotItem->robotModel().info().defaultRightWheelPort().toString();

			if (mUi->rightWheelComboBox->count() > 2) {
				mUi->rightWheelComboBox->setCurrentIndex(2);
			} else if (mUi->rightWheelComboBox->count() > 1) {
				mUi->rightWheelComboBox->setCurrentIndex(1);
			}
		}
	}
}

void TwoDModelWidget::onRobotListChange(RobotItem *robotItem)
{
	if (mScene->oneRobot()) {
		setSelectedRobotItem(mScene->robot(*mModel.robotModels()[0]));
	} else {
		if (mSelectedRobotItem) {
			unsetSelectedRobotItem();
		}
	}

	if (robotItem) {
		connect(&robotItem->robotModel().configuration(), &SensorsConfiguration::deviceAdded
				, [this, robotItem](const PortInfo &port) { reinitSensor(robotItem, port); });

		auto checkAndSaveToRepo = [this](const PortInfo &port, bool isLoaded) {
			Q_UNUSED(port);
			if (!isLoaded) {
				saveToRepo();
			}
		};

		connect(&robotItem->robotModel().configuration(), &SensorsConfiguration::deviceAdded, checkAndSaveToRepo);
		connect(&robotItem->robotModel().configuration(), &SensorsConfiguration::deviceRemoved, checkAndSaveToRepo);
	}
}

void TwoDModelWidget::setSelectedRobotItem(RobotItem *robotItem)
{
	mSelectedRobotItem = robotItem;

	connect(&mSelectedRobotItem->robotModel(), &RobotModel::robotRided, this, &TwoDModelWidget::centerOnRobot);
	connect(&mSelectedRobotItem->robotModel(), &RobotModel::positionChanged, this, &TwoDModelWidget::centerOnRobot);

	setPortsGroupBoxAndWheelComboBoxes();
	updateWheelComboBoxes();

	delete mDisplay;
	mDisplay = mSelectedRobotItem->robotModel().info().displayWidget(this);
	mDisplay->setMinimumSize(displaySize);
	mDisplay->setMaximumSize(displaySize);
	static_cast<QHBoxLayout *>(mUi->displayFrame->layout())->insertWidget(0, mDisplay);
	mUi->displayFrame->setEnabled(true);
	setDisplayVisibility(mDisplayIsVisible);

	mUi->leftWheelComboBox->show();
	mUi->rightWheelComboBox->show();
}

void TwoDModelWidget::unsetSelectedRobotItem()
{
	if (mSelectedRobotItem) {
		unsetPortsGroupBoxAndWheelComboBoxes();
		disconnect(&mSelectedRobotItem->robotModel(), &RobotModel::robotRided, this
				, &TwoDModelWidget::centerOnRobot);
		disconnect(&mSelectedRobotItem->robotModel(), &RobotModel::positionChanged, this
				, &TwoDModelWidget::centerOnRobot);

		mSelectedRobotItem = nullptr;
	}

	static_cast<QHBoxLayout *>(mUi->displayFrame->layout())->removeWidget(mDisplay);
	delete mDisplay;
	mDisplay = new twoDModel::engine::NullTwoDModelDisplayWidget();
	static_cast<QHBoxLayout *>(mUi->displayFrame->layout())->insertWidget(0, mDisplay);

	mUi->displayFrame->setEnabled(false);
}

void TwoDModelWidget::incrementTimelineCounter()
{
	mUi->timelineBox->stepBy(1);
}
