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
#include <QtWidgets/QMessageBox>

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>
#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/qRealFileDialog.h>
#include <qrgui/controller/controllerInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include <kitBase/devicesConfigurationWidget.h>
#include <kitBase/robotModel/robotParts/motor.h>

#include "parts/actionsBox.h"
#include "parts/colorItemPopup.h"
#include "parts/imageItemPopup.h"
#include "parts/robotItemPopup.h"
#include "parts/speedPopup.h"

#include "scene/twoDModelScene.h"
#include "scene/robotItem.h"

#include "src/engine/items/wallItem.h"
#include "src/engine/items/curveItem.h"
#include "src/engine/items/rectangleItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/stylusItem.h"
#include "src/engine/items/imageItem.h"
#include "src/engine/commands/changePropertyCommand.h"

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

const QList<int> speedFactors = { 2, 3, 4, 5, 6, 8, 10, 15, 20 };
const int defaultSpeedFactorIndex = 3;

TwoDModelWidget::TwoDModelWidget(Model &model, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::TwoDModelWidget)
	, mActions(new ActionsBox)
	, mModel(model)
	, mDisplay(new twoDModel::engine::NullTwoDModelDisplayWidget())
	, mNullDisplay(new twoDModel::engine::NullTwoDModelDisplayWidget())
	, mCurrentSpeed(defaultSpeedFactorIndex)
	, mNoneCursorType(noDrag)
	, mCursorType(noDrag)
{
	setWindowIcon(QIcon(":/icons/2d-model.svg"));

	initWidget();
	initPalette();

	connectUiButtons();

	mUi->detailsTab->setPhysicsSettings(mUi->physicsFrame);
	mUi->realisticPhysicsCheckBox->setChecked(mModel.settings().realisticPhysics());
	mUi->enableSensorNoiseCheckBox->setChecked(mModel.settings().realisticSensors());
	mUi->enableMotorNoiseCheckBox->setChecked(mModel.settings().realisticMotors());
	changePhysicsSettings();

	connect(mScene, &TwoDModelScene::selectionChanged, this, &TwoDModelWidget::onSelectionChange);
	connect(mScene, &TwoDModelScene::mousePressed, this, &TwoDModelWidget::refreshCursor);
	connect(mScene, &TwoDModelScene::mouseReleased, this, &TwoDModelWidget::refreshCursor);
	connect(mScene, &TwoDModelScene::mouseReleased, this, [this](){ saveToRepo(); });
	connect(mScene, &TwoDModelScene::robotPressed, mUi->palette, &Palette::unselect);
	connect(mScene, &TwoDModelScene::robotListChanged, this, &TwoDModelWidget::onRobotListChange);

	connect(&mModel.worldModel(), &WorldModel::backgroundChanged, mScene, &TwoDModelScene::setBackground);
	connect(&mModel.timeline(), &Timeline::started, [this]() {
		if (mRobotPositionReadOnly) {
			returnToStartMarker();
		}
	});
	connect(&mModel.timeline(), &Timeline::started, [this]() { bringToFront(); mUi->timelineBox->setValue(0); });
	connect(&mModel.timeline(), &Timeline::tick, this, &TwoDModelWidget::incrementTimelineCounter);
	connect(&mModel.timeline(), &Timeline::started, this, &TwoDModelWidget::setRunStopButtonsVisibility);
	connect(&mModel.timeline(), &Timeline::stopped, this, &TwoDModelWidget::setRunStopButtonsVisibility);
	connect(&mModel.timeline(), &Timeline::speedFactorChanged, [=](int value) {
		const QPoint downCoords = mUi->speedDownButton->mapTo(this, mUi->speedDownButton->rect().bottomRight());
		const QPoint upCoords = mUi->speedUpButton->mapTo(this, mUi->speedUpButton->rect().bottomLeft());
		const QPoint coords((downCoords.x() + upCoords.x() - mSpeedPopup->width()) / 2, downCoords.y() + 10);
		mSpeedPopup->move(coords);
		// Setting value in percents
		mSpeedPopup->setSpeed(100 / speedFactors[defaultSpeedFactorIndex] * value);
	});
	setRunStopButtonsVisibility();

	mUi->palette->unselect();

	setFocus();

	mModel.timeline().setSpeedFactor(speedFactors[defaultSpeedFactorIndex]);
	checkSpeedButtons();
	mUi->timelineBox->setSingleStep(Timeline::timeInterval * 0.001);

	mUi->horizontalRuler->setScene(mUi->graphicsView);
	mUi->verticalRuler->setScene(mUi->graphicsView);
	mUi->horizontalRuler->setPixelsInCm(pixelsInCm);
	mUi->verticalRuler->setPixelsInCm(pixelsInCm);

	/// @todo: make some values editable
	mUi->detailsTab->setParamsSettings(mUi->physicsParamsFrame);
	mUi->wheelDiamInCm->setValue(robotWheelDiameterInCm);
	mUi->robotHeightInCm->setValue(robotHeight / pixelsInCm); // Not sure if correct
	mUi->robotWidthInCm->setValue(robotWidth / pixelsInCm);
	mUi->robotMassInGr->setValue(robotMass);
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

	mScene = new TwoDModelScene(mModel, mUi->graphicsView);
	connectDevicesConfigurationProvider(mScene);
	mScene->addActions(mActions->sceneContextMenuActions());
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	move(0, 0);

	QPen defaultPen(Qt::black);
	defaultPen.setWidth(defaultPenWidth);
	// Popups will listen to scene events, appear, disappear and free itself.
	mColorFieldItemPopup = new ColorItemPopup(defaultPen, *mScene, this);
	mImageItemPopup = new ImageItemPopup(*mScene, this);
	mRobotItemPopup = new RobotItemPopup(*mScene, this);
	mSpeedPopup = new SpeedPopup(this);

	mScene->setPenBrushItems(defaultPen, Qt::NoBrush);
	connect(mColorFieldItemPopup, &ColorItemPopup::userPenChanged, [=](const QPen &pen) {
		mScene->setPenBrushItems(pen, QBrush(pen.color(), Qt::NoBrush));
	});
	connect(mColorFieldItemPopup, &ColorItemPopup::propertyChanged, this, &TwoDModelWidget::saveToRepo);

	connect(mImageItemPopup, &ImageItemPopup::somethingChanged, this, &TwoDModelWidget::saveToRepo);

	connect(mSpeedPopup, &SpeedPopup::resetToDefault, this, [=]() {
		mCurrentSpeed = defaultSpeedFactorIndex;
		mModel.timeline().setSpeedFactor(speedFactors[defaultSpeedFactorIndex]);
	});

	mDisplay->setMinimumSize(displaySize);
	mDisplay->setMaximumSize(displaySize);
	mUi->detailsTab->setDisplay(mDisplay);

	auto toggleRulers = [=]() {
		const bool gridVisible = SettingsManager::value("2dShowGrid").toBool();
		mUi->horizontalRuler->setVisible(gridVisible);
		mUi->verticalRuler->setVisible(gridVisible);
	};
	toggleRulers();

	connect(mUi->gridParametersBox, SIGNAL(parametersChanged()), mScene, SLOT(update()));
	connect(mUi->gridParametersBox, &GridParameters::parametersChanged, toggleRulers);
	connect(mUi->gridParametersBox, SIGNAL(parametersChanged()), mUi->horizontalRuler, SLOT(update()));
	connect(mUi->gridParametersBox, SIGNAL(parametersChanged()), mUi->verticalRuler, SLOT(update()));
	connect(mScene, SIGNAL(sceneRectChanged(QRectF)), mUi->horizontalRuler, SLOT(update()));
	connect(mScene, SIGNAL(sceneRectChanged(QRectF)), mUi->verticalRuler, SLOT(update()));
	connect(mScene, &AbstractScene::focused, this, [=]() { onFocusIn(); });
	connect(mScene->mainView(), SIGNAL(zoomChanged()), mUi->horizontalRuler, SLOT(update()));
	connect(mScene->mainView(), SIGNAL(zoomChanged()), mUi->verticalRuler, SLOT(update()));
	connect(mScene->mainView(), SIGNAL(contentsRectChanged()), mUi->horizontalRuler, SLOT(update()));
	connect(mScene->mainView(), SIGNAL(contentsRectChanged()), mUi->verticalRuler, SLOT(update()));
}

void TwoDModelWidget::initPalette()
{
	QAction * const wallTool = items::WallItem::wallTool();
	QAction * const lineTool = items::LineItem::lineTool();
	QAction * const bezierTool = items::CurveItem::curveTool();
	QAction * const rectangleTool = items::RectangleItem::rectangleTool();
	QAction * const ellipseTool = items::EllipseItem::ellipseTool();
	QAction * const stylusTool = items::StylusItem::stylusTool();
	QAction * const imageTool = items::ImageItem::imageTool();

	mUi->palette->registerTool(wallTool);
	mUi->palette->registerTool(lineTool);
	mUi->palette->registerTool(bezierTool);
	mUi->palette->registerTool(rectangleTool);
	mUi->palette->registerTool(ellipseTool);
	mUi->palette->registerTool(stylusTool);
	mUi->palette->registerTool(imageTool);

	connect(wallTool, &QAction::triggered, mScene, &TwoDModelScene::addWall);
	connect(lineTool, &QAction::triggered, mScene, &TwoDModelScene::addLine);
	connect(bezierTool, &QAction::triggered, mScene, &TwoDModelScene::addBezier);
	connect(rectangleTool, &QAction::triggered, mScene, &TwoDModelScene::addRectangle);
	connect(ellipseTool, &QAction::triggered, mScene, &TwoDModelScene::addEllipse);
	connect(stylusTool, &QAction::triggered, mScene, &TwoDModelScene::addStylus);
	connect(imageTool, &QAction::triggered, mScene, &TwoDModelScene::addImage);
	connect(&mUi->palette->cursorAction(), &QAction::triggered, mScene, &TwoDModelScene::setNoneStatus);

	connect(wallTool, &QAction::triggered, [this](){ setCursorTypeForDrawing(drawWall); });
	connect(lineTool, &QAction::triggered, [this](){ setCursorTypeForDrawing(drawLine); });
	connect(ellipseTool, &QAction::triggered, [this](){ setCursorTypeForDrawing(drawEllipse); });
	connect(stylusTool, &QAction::triggered, [this](){ setCursorTypeForDrawing(drawStylus); });
	connect(&mUi->palette->cursorAction(), &QAction::triggered, [this](){ setCursorTypeForDrawing(mNoneCursorType); });
}

void TwoDModelWidget::initDetailsTab()
{
}

void TwoDModelWidget::connectUiButtons()
{
	connect(mUi->realisticPhysicsCheckBox, &QAbstractButton::toggled, this, &TwoDModelWidget::changePhysicsSettings);
	connect(mUi->enableMotorNoiseCheckBox, &QAbstractButton::toggled, this, &TwoDModelWidget::changePhysicsSettings);
	connect(mUi->enableSensorNoiseCheckBox, &QAbstractButton::toggled, this, &TwoDModelWidget::changePhysicsSettings);

	connect(&mActions->deleteAllAction(), &QAction::triggered, [this](){
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
			, &mActions->clearFloorAction(), &QAction::setEnabled, Qt::QueuedConnection);

	connect(&mActions->saveModelAction(), &QAction::triggered, this, &TwoDModelWidget::saveWorldModel);
	connect(&mActions->loadModelAction(), &QAction::triggered, this, &TwoDModelWidget::loadWorldModel);
	connect(&mActions->setBackgroundAction(), &QAction::triggered, this, &TwoDModelWidget::setBackground);

	connect(mUi->speedUpButton, &QAbstractButton::clicked, this, &TwoDModelWidget::speedUp);
	connect(mUi->speedDownButton, &QAbstractButton::clicked, this, &TwoDModelWidget::speedDown);

	connect(mRobotItemPopup, &RobotItemPopup::followingChanged, this, &TwoDModelWidget::enableRobotFollowing);
	connect(&mActions->scrollHandModeAction(), &QAction::triggered
			, this, &TwoDModelWidget::onHandCursorActionTriggered);
	connect(&mActions->multiSelectionModeAction(), &QAction::triggered
			, this, &TwoDModelWidget::onMultiselectionCursorActionTriggered);

	connect(mRobotItemPopup, &RobotItemPopup::restoreRobotPositionClicked, this, &TwoDModelWidget::returnToStartMarker);
	connect(mUi->initialStateButton, &QAbstractButton::clicked, this, &TwoDModelWidget::returnToStartMarker);
	connect(mUi->toggleDetailsButton, &QAbstractButton::clicked, this, &TwoDModelWidget::toggleDetailsVisibility);

	connect(mUi->trainingModeButton, &QAbstractButton::toggled, this, &TwoDModelWidget::trainingModeChanged);
	mUi->trainingModeButton->setChecked(false);

	initRunStopButtons();
}

void TwoDModelWidget::setPortsGroupBoxAndWheelComboBoxes()
{
	mCurrentConfigurer = new DevicesConfigurationWidget(mUi->portsFrame, true, true);
	mCurrentConfigurer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	mCurrentConfigurer->loadRobotModels({ &mSelectedRobotItem->robotModel().info() });
	mCurrentConfigurer->selectRobotModel(mSelectedRobotItem->robotModel().info());
	mUi->portsFrame->layout()->addWidget(mCurrentConfigurer);
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

	mUi->detailsTab->setDevicesConfigurer(mUi->portsFrame);
	mUi->detailsTab->setMotorsConfigurer(mUi->motorsFrame);
}

void TwoDModelWidget::unsetPortsGroupBoxAndWheelComboBoxes()
{
	if (mCurrentConfigurer) {
		mUi->portsFrame->layout()->removeWidget(mCurrentConfigurer);
		mCurrentConfigurer->disconnectDevicesConfigurationProvider();

		if (mSelectedRobotItem) {
			mSelectedRobotItem->robotModel().configuration().disconnectDevicesConfigurationProvider();
		}

		delete mCurrentConfigurer;
	}
}

void TwoDModelWidget::returnToStartMarker()
{
	mModel.worldModel().clearRobotTrace();
	for (RobotModel * const model : mModel.robotModels()) {
		mScene->robot(*model)->returnToStartPosition();
	}
}

void TwoDModelWidget::trainingModeChanged(bool enabled)
{
	mUi->trainingModeButton->setToolTip(enabled
			? tr("Training mode: solution will not be checked")
			: tr("Checking mode: solution will be checked, errors will be reported"));
	mModel.setConstraintsEnabled(!enabled);
}

void TwoDModelWidget::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);
	if (event->key() == Qt::Key_F5) {
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
	enableRobotFollowing(SettingsManager::value("2dFollowingRobot").toBool());
	setCursorType(static_cast<CursorType>(SettingsManager::value("2dCursorType").toInt()));
	setDetailsVisibility(SettingsManager::value("2d_detailsVisible").toBool());
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

void TwoDModelWidget::setBackground()
{
	// Loads world and robot models simultaneously.
	const QString loadFileName = QRealFileDialog::getOpenFileName("2DSelectBackground", this
			, tr("Select background image"), "./fields", tr("Graphics (*.*)"));
	if (loadFileName.isEmpty()) {
		return;
	}

	const Image image(loadFileName, false);
	const QSize size = image.preferedSize();
	mModel.worldModel().setBackground(image, QRect(QPoint(-size.width()/2, -size.height()/2), size));
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

void TwoDModelWidget::onSelectionChange()
{
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

void TwoDModelWidget::speedUp()
{
	if (mCurrentSpeed < speedFactors.count() - 1) {
		mModel.timeline().setSpeedFactor(speedFactors[++mCurrentSpeed]);
		checkSpeedButtons();
	}
}

void TwoDModelWidget::speedDown()
{
	if (mCurrentSpeed > 0) {
		mModel.timeline().setSpeedFactor(speedFactors[--mCurrentSpeed]);
		checkSpeedButtons();
	}
}

void TwoDModelWidget::checkSpeedButtons()
{
	mUi->speedUpButton->setEnabled(mCurrentSpeed < speedFactors.count() - 1);
	mUi->speedDownButton->setEnabled(mCurrentSpeed > 0);
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

void TwoDModelWidget::focusInEvent(QFocusEvent *event)
{
	QWidget::focusInEvent(event);
	onFocusIn();
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
	if (mController) {
		// Clearing 2D model undo stack...
		mController->moduleClosed(editorId());
		mController->moduleOpened(editorId());
	}

	mScene->clearScene(true, Reason::loading);
	mModel.deserialize(worldModel);
	updateWheelComboBoxes();
	mUi->trainingModeButton->setVisible(mModel.hasConstraints());
}

Model &TwoDModelWidget::model() const
{
	return mModel;
}

void TwoDModelWidget::setController(ControllerInterface &controller)
{
	mController = &controller;
	mScene->setController(controller);

	auto setItemsProperty = [=](const QStringList &items, const QString &property, const QVariant &value) {
		if (mController) {
			mController->execute(new commands::ChangePropertyCommand(*mScene, mModel, items, property, value));
		}
	};

	connect(mRobotItemPopup, &graphicsUtils::ItemPopup::propertyChanged, this, setItemsProperty);
	connect(mColorFieldItemPopup, &graphicsUtils::ItemPopup::propertyChanged, this, setItemsProperty);
}

void TwoDModelWidget::setInteractivityFlags(ReadOnlyFlags flags)
{
	const bool worldReadOnly = (flags & ReadOnly::World) != 0;

	mUi->palette->setVisible(!worldReadOnly);
	mActions->setWorldModelActionsVisible(!worldReadOnly);
	mColorFieldItemPopup->setEnabled(!worldReadOnly);

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

	const bool sensorsReadOnly = flags.testFlag(ReadOnly::Sensors);
	const bool robotConfigurationReadOnly = flags.testFlag(ReadOnly::RobotSetup);

	mUi->detailsTab->setDevicesSectionsVisible(!sensorsReadOnly);
	mUi->detailsTab->setMotorsSectionsVisible(!robotConfigurationReadOnly);

	mCurrentConfigurer->setEnabled(!sensorsReadOnly);
	mUi->leftWheelComboBox->setEnabled(!robotConfigurationReadOnly);
	mUi->rightWheelComboBox->setEnabled(!robotConfigurationReadOnly);

	const bool simulationSettingsReadOnly = flags.testFlag(ReadOnly::SimulationSettings);

	mUi->detailsTab->setPhysicsSectionsVisible(!simulationSettingsReadOnly);

	mSensorsReadOnly = sensorsReadOnly;
	mRobotPositionReadOnly = flags.testFlag(ReadOnly::RobotPosition);

	mScene->setInteractivityFlags(flags);
}

void TwoDModelWidget::setCompactMode(bool enabled)
{
	mCompactMode = enabled;
	setRunStopButtonsVisibility();
	mActions->setSaveLoadActionsShortcutsEnabled(!mCompactMode);
}

QString TwoDModelWidget::editorId() const
{
	return "TrikStudio.2DModel.Editor";
}

bool TwoDModelWidget::supportsZooming() const
{
	return true;
}

void TwoDModelWidget::configure(QAction &zoomIn, QAction &zoomOut, QAction &undo, QAction &redo
		, QAction &copy, QAction &paste, QAction &cut)
{
	EditorInterface::configure(zoomIn, zoomOut, undo, redo, copy, paste, cut);
	addActions({ mZoomInAction, mZoomOutAction, mUndoAction, mRedoAction, mCopyAction, mPasteAction, mCutAction });
}

void TwoDModelWidget::zoomIn()
{
	mScene->mainView()->zoomIn();
}

void TwoDModelWidget::zoomOut()
{
	mScene->mainView()->zoomOut();
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

void TwoDModelWidget::toggleDetailsVisibility()
{
	setDetailsVisibility(!mDetailsAreVisible);
}

void TwoDModelWidget::setDetailsVisibility(bool visible)
{
	mDetailsAreVisible = visible;
	mUi->detailsContainer->setVisible(visible);
	const QString direction = visible ? "right" : "left";
	mUi->toggleDetailsButton->setIcon(QIcon(QString(":/icons/2d_%1.png").arg(direction)));
	mUi->toggleDetailsButton->setFlat(visible);
	mUi->toggleDetailsButton->setToolTip(visible ? tr("Hide details") : tr("Show details"));
	SettingsManager::setValue("2d_detailsVisible", visible);
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

void TwoDModelWidget::onHandCursorActionTriggered()
{
	setCursorType(hand);
	mUi->palette->unselect();
}

void TwoDModelWidget::onMultiselectionCursorActionTriggered()
{
	setCursorType(multiselection);
	mUi->palette->unselect();
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

bool TwoDModelWidget::setSelectedPort(QComboBox * const comboBox, const PortInfo &port)
{
	for (int i = 0; i < comboBox->count(); ++i) {
		if (comboBox->itemData(i).value<PortInfo>() == port) {
			comboBox->setCurrentIndex(i);
			return true;
		}
	}

	return false;
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

	if (!setSelectedPort(mUi->leftWheelComboBox, leftWheelOldPort)) {
		if (!setSelectedPort(mUi->leftWheelComboBox
				, mSelectedRobotItem->robotModel().info().defaultLeftWheelPort())) {
			qWarning() << "Incorrect defaultLeftWheelPort set in configurer:"
					<< mSelectedRobotItem->robotModel().info().defaultLeftWheelPort().toString();

			if (mUi->leftWheelComboBox->count() > 1) {
				mUi->leftWheelComboBox->setCurrentIndex(1);
			}
		}
	}

	if (!setSelectedPort(mUi->rightWheelComboBox, rightWheelOldPort)) {
		if (!setSelectedPort(mUi->rightWheelComboBox
				, mSelectedRobotItem->robotModel().info().defaultRightWheelPort())) {

			qWarning() << "Incorrect defaultRightWheelPort set in configurer:"
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
				, [this, robotItem](const PortInfo &port) { mScene->reinitSensor(robotItem, port); });

		auto checkAndSaveToRepo = [this](const PortInfo &port, bool isLoaded) {
			Q_UNUSED(port);
			if (!isLoaded) {
				saveToRepo();
			}
		};

		connect(&robotItem->robotModel().configuration(), &SensorsConfiguration::deviceAdded, checkAndSaveToRepo);
		connect(&robotItem->robotModel().configuration(), &SensorsConfiguration::deviceRemoved, checkAndSaveToRepo);

		connect(&robotItem->robotModel(), &RobotModel::wheelOnPortChanged
				, [=](RobotModel::WheelEnum wheel, const PortInfo &port)
		{
			if (port.isValid()) {
				setSelectedPort(wheel == RobotModel::WheelEnum::left
						? mUi->leftWheelComboBox : mUi->rightWheelComboBox, port);
			}
		});
	}
}

void TwoDModelWidget::setSelectedRobotItem(RobotItem *robotItem)
{
	mSelectedRobotItem = robotItem;

	connect(&mSelectedRobotItem->robotModel(), &RobotModel::robotRided, this, &TwoDModelWidget::centerOnRobot);
	connect(&mSelectedRobotItem->robotModel(), &RobotModel::positionChanged, this, &TwoDModelWidget::centerOnRobot);

	setPortsGroupBoxAndWheelComboBoxes();
	updateWheelComboBoxes();

	mUi->detailsTab->setDisplay(nullptr);
	mDisplay = mSelectedRobotItem->robotModel().info().displayWidget();
	mDisplay->setParent(this);
	mDisplay->setMinimumSize(displaySize);
	mDisplay->setMaximumSize(displaySize);
	mUi->detailsTab->setDisplay(mDisplay);

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

	mUi->detailsTab->setDisplay(nullptr);
	mDisplay = mNullDisplay;
	mUi->detailsTab->setDisplay(mDisplay);
}

void TwoDModelWidget::incrementTimelineCounter()
{
	mUi->timelineBox->stepBy(1);
}
