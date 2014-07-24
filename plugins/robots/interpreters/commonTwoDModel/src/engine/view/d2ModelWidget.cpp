#include "d2ModelWidget.h"
#include "ui_d2Form.h"

#include <QtCore/qmath.h>
#include <QtCore/QDebug>

#include <qrkernel/settingsManager.h>
#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/qRealFileDialog.h>

#include <interpreterBase/devicesConfigurationWidget.h>
#include <interpreterBase/robotModel/robotParts/motor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

#include "sensorItem.h"
#include "sonarSensorItem.h"
#include "rotater.h"

#include "src/engine/items/wallItem.h"
#include "src/engine/items/ellipseItem.h"
#include "src/engine/items/stylusItem.h"

#include "src/engine/model/constants.h"
#include "src/engine/model/model.h"

using namespace twoDModel;
using namespace view;
using namespace model;
using namespace qReal;
using namespace utils;
using namespace graphicsUtils;
using namespace interpreterBase;
using namespace interpreterBase::robotModel;
using namespace robotParts;

D2ModelWidget::D2ModelWidget(Model &model, QWidget *parent)
	: QRealDialog("D2ModelWindow", parent)
	, mUi(new Ui::D2Form)
	, mScene(nullptr)
	, mModel(model)
	, mDisplay(nullptr)
	, mWidth(defaultPenWidth)
	, mFirstShow(true)
{
	setWindowIcon(QIcon(":/icons/2d-model.svg"));

	initWidget();

	connectUiButtons();
	mUi->realisticPhysicsCheckBox->setChecked(mModel.settings().realisticPhysics());
	mUi->enableSensorNoiseCheckBox->setChecked(mModel.settings().realisticSensors());
	mUi->enableMotorNoiseCheckBox->setChecked(mModel.settings().realisticMotors());
	changePhysicsSettings();

	connect(mScene, &D2ModelScene::selectionChanged, this, &D2ModelWidget::changePalette);
	connect(mScene, &D2ModelScene::mousePressed, this, &D2ModelWidget::refreshCursor);
	connect(mScene, &D2ModelScene::mouseReleased, this, &D2ModelWidget::refreshCursor);
	connect(mScene, &D2ModelScene::mouseReleased, this, &D2ModelWidget::saveToRepo);
	connect(mScene, &D2ModelScene::robotPressed, [this]() { mUi->noneButton->setChecked(true); });

	connect(&mModel.timeline(), &Timeline::started, [this]() { mUi->timelineBox->setValue(0); });

	connect(&mModel.timeline(), &Timeline::tick, [this]() { mUi->timelineBox->stepBy(1); });

	setCursorType(static_cast<CursorType>(SettingsManager::value("2dCursorType").toInt()));
	syncCursorButtons();
	enableRobotFollowing(SettingsManager::value("2dFollowingRobot").toBool());
	mUi->autoCenteringButton->setChecked(mFollowRobot);
	mUi->noneButton->setChecked(true);

	setFocus();

	mUi->timelineBox->setSingleStep(Timeline::timeInterval * 0.001);
}

D2ModelWidget::~D2ModelWidget()
{
	delete mScene;
	delete mUi;
}

void D2ModelWidget::initWidget()
{
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

	mUi->setupUi(this);

	mScene = new D2ModelScene(mModel, mUi->graphicsView);
	connectDevicesConfigurationProvider(mScene);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	move(0, 0);

	mUi->penWidthSpinBox->setRange(1, 30);

	QStringList const colorList = { "Black", "Blue", "Green", "Yellow", "Red" };
	QStringList const translatedColorList = { tr("Black"), tr("Blue"), tr("Green"), tr("Yellow"), tr("Red") };

	mUi->penColorComboBox->setColorList(colorList, translatedColorList);
	mUi->penColorComboBox->setColor(QColor("black"));

	initButtonGroups();

	if (mDisplay) {
		mDisplay->setMinimumSize(displaySize);
		mDisplay->setMaximumSize(displaySize);
		dynamic_cast<QHBoxLayout *>(mUi->displayFrame->layout())->insertWidget(0, mDisplay);
		mUi->displayFrame->setVisible(true);
	} else {
		mUi->displayFrame->setVisible(false);
	}

	setDisplayVisibility(SettingsManager::value("2d_displayVisible").toBool());

	connect(mUi->gridParametersBox, SIGNAL(parametersChanged()), mScene, SLOT(update()));
	connect(mUi->gridParametersBox, &GridParameters::parametersChanged, mScene, &D2ModelScene::alignWalls);
}

void D2ModelWidget::connectUiButtons()
{
	connect(mUi->realisticPhysicsCheckBox, &QAbstractButton::toggled, this, &D2ModelWidget::changePhysicsSettings);
	connect(mUi->enableMotorNoiseCheckBox, &QAbstractButton::toggled, this, &D2ModelWidget::changePhysicsSettings);
	connect(mUi->enableSensorNoiseCheckBox, &QAbstractButton::toggled, this, &D2ModelWidget::changePhysicsSettings);

	connect(mUi->ellipseButton, &QAbstractButton::toggled, mScene, &D2ModelScene::addEllipse);
	connect(mUi->stylusButton, &QAbstractButton::toggled, mScene, &D2ModelScene::addStylus);
	connect(mUi->lineButton, &QAbstractButton::toggled, mScene, &D2ModelScene::addLine);
	connect(mUi->wallButton, &QAbstractButton::toggled, mScene, &D2ModelScene::addWall);
	connect(mUi->noneButton, &QAbstractButton::toggled, mScene, &D2ModelScene::setNoneStatus);

	connect(mUi->ellipseButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawEllipse); });
	connect(mUi->stylusButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawStylus); });
	connect(mUi->lineButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawLine); });
	connect(mUi->wallButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(drawWall); });
	connect(mUi->noneButton, &QAbstractButton::toggled, [this](){ setCursorTypeForDrawing(mNoneCursorType); });

	connect(mUi->clearButton, &QAbstractButton::clicked, mScene, &D2ModelScene::clearScene);
	connect(&mButtonGroup, static_cast<void (QButtonGroup::*)(QAbstractButton *, bool)>(&QButtonGroup::buttonToggled)
			, [this](QAbstractButton *button, bool toggled) {
				if (toggled) {
					setHighlightOneButton(button);
				} else if (!mButtonGroup.checkedButton()) {
					setHighlightOneButton(mUi->noneButton);
				}
	});

	connect(mUi->penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
	connect(mUi->penColorComboBox, SIGNAL(activated(int)), this, SLOT(changePenColor(int)));

	connect(mUi->saveWorldModelPushButton, SIGNAL(clicked()), this, SLOT(saveWorldModel()));
	connect(mUi->loadWorldModelPushButton, SIGNAL(clicked()), this, SLOT(loadWorldModel()));

	connect(mUi->speedComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSpeed(int)));

	connect(mUi->autoCenteringButton, SIGNAL(toggled(bool)), this, SLOT(enableRobotFollowing(bool)));
	connect(mUi->handCursorButton, SIGNAL(toggled(bool)), this, SLOT(onHandCursorButtonToggled(bool)));
	connect(mUi->multiselectionCursorButton, SIGNAL(toggled(bool)), this, SLOT(onMultiselectionCursorButtonToggled(bool)));

	connect(mUi->initialStateButton, SIGNAL(clicked()), this, SLOT(setInitialRobotBeforeRun()));
	connect(mUi->displayButton, SIGNAL(clicked()), this, SLOT(toggleDisplayVisibility()));

	initRunStopButtons();
}

void D2ModelWidget::initButtonGroups()
{
	mButtonGroup.setExclusive(false);
	mButtonGroup.addButton(mUi->lineButton);
	mButtonGroup.addButton(mUi->wallButton);
	mButtonGroup.addButton(mUi->stylusButton);
	mButtonGroup.addButton(mUi->ellipseButton);
	mButtonGroup.addButton(mUi->noneButton);

	mCursorButtonGroup.setExclusive(true);
	mCursorButtonGroup.addButton(mUi->handCursorButton);
	mCursorButtonGroup.addButton(mUi->multiselectionCursorButton);
}

void D2ModelWidget::setPortsGroupBoxAndWheelComboBoxes()
{
	qDebug() << "setPortsGroupBoxAndWheelComboBoxes() in";
	mCurrentConfigurer = new DevicesConfigurationWidget(mUi->portsGroupBox, true, true);
	mCurrentConfigurer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	mCurrentConfigurer->loadRobotModels({ mSelectedRobotItem->robotModel().info() });
	mCurrentConfigurer->selectRobotModel(*mSelectedRobotItem->robotModel().info());
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
	qDebug() << "setPortsGroupBoxAndWheelComboBoxes() out";
}

void D2ModelWidget::unsetPortsGroupBoxAndWheelComboBoxes()
{
	qDebug() << "unsetPortsGroupBoxAndWheelComboBoxes() in";
	if (mCurrentConfigurer) {
		mUi->portsGroupBox->layout()->removeWidget(mCurrentConfigurer);

		if (mSelectedRobotItem) {
			mSelectedRobotItem->robotModel().configuration().disconnectDevicesConfigurationProvider();
		}

		delete mCurrentConfigurer;
	}
	qDebug() << "unsetPortsGroupBoxAndWheelComboBoxes() out";
}

void D2ModelWidget::setHighlightOneButton(QAbstractButton * const oneButton)
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

void D2ModelWidget::changeSpeed(int curIndex)
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

void D2ModelWidget::init()
{
	mUi->graphicsView->show();
	if (isHidden()) {
		show();
	}

	if (!isActiveWindow()) {
		activateWindow();
	}

	update();
	updateWheelComboBoxes();
}

void D2ModelWidget::saveInitialRobotBeforeRun()
{
	qDebug() << "saveInitialRobotBeforeRun() in";
	for (RobotModel *robotModel : mModel.robotModels()) {
		RobotState state;
		state.pos = robotModel->position();
		state.rotation = robotModel->rotation();
		mInitialRobotsBeforeRun.insert(robotModel, state);
	}
	qDebug() << "saveInitialRobotBeforeRun() out";
}

void D2ModelWidget::setInitialRobotBeforeRun()
{
	qDebug() << "setInitialRobotBeforeRun() in";
	QMapIterator<RobotModel *, RobotState> i(mInitialRobotsBeforeRun);

	while (i.hasNext()) {
		i.next();
		i.key()->setPosition(i.value().pos);
		i.key()->setRotation(i.value().rotation);
	}
	qDebug() << "setInitialRobotBeforeRun() out";
}

void D2ModelWidget::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);
	if (event->matches(QKeySequence::ZoomIn)) {
		mScene->mainView()->zoomIn();
	} else if (event->matches(QKeySequence::ZoomOut)) {
		mScene->mainView()->zoomOut();
	} else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Space) {
		mUi->runButton->animateClick();
	} else if (event->key() == Qt::Key_Escape) {
		mUi->stopButton->animateClick();
	}
}

void D2ModelWidget::close()
{
	setVisible(false);
}

void D2ModelWidget::changeEvent(QEvent *e)
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

void D2ModelWidget::showEvent(QShowEvent *e)
{
	e->accept();
	QRealDialog::showEvent(e);
	if (mFirstShow) {
		mFirstShow = false;
		onFirstShow();
	}
}

void D2ModelWidget::onFirstShow()
{
	mUi->speedComboBox->setCurrentIndex(1); // Normal speed
}

void D2ModelWidget::centerOnRobot()
{
	qDebug() << "centerOnRobot() in";
	if (mSelectedRobotItem && mFollowRobot && mSelectedRobotItem->robotModel().onTheGround()) {
		mScene->centerOnRobot(mSelectedRobotItem);
	}
	qDebug() << "centerOnRobot() out";
}

void D2ModelWidget::setCursorTypeForDrawing(CursorType type)
{
	mCursorType = type;
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));
}

void D2ModelWidget::saveWorldModel()
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

	QDomDocument const save = generateXml();

	utils::OutFile saveFile(saveFileName);
	saveFile() << "<?xml version='1.0' encoding='utf-8'?>\n";
	saveFile() << save.toString(4);
}

void D2ModelWidget::loadWorldModel()
{
	qDebug() << "loadWorldModel() in";
	// Loads world and robot models simultaneously.
	QString const loadFileName = QRealFileDialog::getOpenFileName("Open2DModelWidget", this
			, tr("Loading world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (loadFileName.isEmpty()) {
		return;
	}

	QDomDocument const save = utils::xmlUtils::loadDocument(loadFileName);
	loadXml(save);
	qDebug() << "loadWorldModel() out";
}

bool D2ModelWidget::isColorItem(AbstractItem * const item) const
{
	return dynamic_cast<items::ColorFieldItem *>(item)
			&& !dynamic_cast<items::WallItem *>(item);
}

QList<AbstractItem *> D2ModelWidget::selectedColorItems() const
{
	QList<AbstractItem *> resList;
	for (QGraphicsItem * const graphicsItem : mScene->selectedItems()) {
		AbstractItem *item = dynamic_cast<AbstractItem*>(graphicsItem);
		if (item && isColorItem(item)) {
			resList << item;
		}
	}

	return resList;
}

void D2ModelWidget::changePenWidth(int width)
{
	mScene->setPenWidthItems(width);
	mWidth = width;
	for (AbstractItem * const item : selectedColorItems()) {
		item->setPenWidth(width);
	}

	mScene->update();
}

void D2ModelWidget::changePenColor(int textIndex)
{
	QString const text = mUi->penColorComboBox->colorByIndex(textIndex).name();
	mScene->setPenColorItems(text);
	for (AbstractItem * const item : selectedColorItems()) {
		item->setPenColor(text);
	}

	mScene->update();
}

void D2ModelWidget::changePalette()
{
	QList<QGraphicsItem *> const listSelectedItems = mScene->selectedItems();
	if (listSelectedItems.isEmpty()) {
		setNoPalette();
		mScene->setEmptyPenBrushItems();
	} else {
		AbstractItem *item = dynamic_cast<AbstractItem *>(listSelectedItems.back());
		if (isColorItem(item)) {
			QPen const penItem = item->pen();
			QBrush const brushItem = item->brush();
			setItemPalette(penItem, brushItem);
			mScene->setPenBrushItems(penItem, brushItem);
		}
	}
}

void D2ModelWidget::onSelectionChange()
{
	qDebug() << "onSelectionChange() in";
	changePalette();

	QList<QGraphicsItem *> listSelectedItems = mScene->selectedItems();
	RobotItem *robotItem = nullptr;
	bool oneRobotItem = false;

	for (QGraphicsItem *item : listSelectedItems) {
		if (static_cast<RobotItem *>(item) != nullptr) {
			if (!oneRobotItem) {
				robotItem = static_cast<RobotItem *>(item);
				oneRobotItem = true;
			} else {
				if (mSelectedRobotItem) {
					unsetPortsGroupBoxAndWheelComboBoxes();
					mSelectedRobotItem = nullptr;
					mUi->leftWheelComboBox->hide();
					mUi->rightWheelComboBox->hide();
				}
				return;
			}
		}
	}

	if (oneRobotItem) {
		if (mSelectedRobotItem
				&& robotItem->robotModel().info()->robotId() == mSelectedRobotItem->robotModel().info()->robotId()) {
			return;
		}

		if (mSelectedRobotItem) {
			unsetPortsGroupBoxAndWheelComboBoxes();
			disconnect(&robotItem->robotModel(), &RobotModel::positionChanged, this
					, &D2ModelWidget::centerOnRobot);
			mSelectedRobotItem = nullptr;
		}

		if (mDisplay) {
			delete mDisplay;
		}

		if (robotItem->robotModel().info()->name() == "NullTwoDRobotModel") {
			return;
		}

		mSelectedRobotItem = robotItem;

		connect(&robotItem->robotModel(), &RobotModel::positionChanged, this, &D2ModelWidget::centerOnRobot);

		setPortsGroupBoxAndWheelComboBoxes();
		updateWheelComboBoxes();


		mDisplay = mSelectedRobotItem->robotModel().info()->displayWidget(this);
		connect(&mModel.timeline(), &Timeline::started, mDisplay, &engine::TwoDModelDisplayWidget::clear);

		mUi->leftWheelComboBox->show();
		mUi->rightWheelComboBox->show();
	}
	qDebug() << "onSelectionChange() out";
}

void D2ModelWidget::setValuePenColorComboBox(QColor const &penColor)
{
	mUi->penColorComboBox->setColor(penColor);
}

void D2ModelWidget::setValuePenWidthSpinBox(int width)
{
	mUi->penWidthSpinBox->setValue(width);
}

void D2ModelWidget::setItemPalette(QPen const &penItem, QBrush const &brushItem)
{
	Q_UNUSED(brushItem)
	setValuePenWidthSpinBox(penItem.width());
	setValuePenColorComboBox(penItem.color());
}

void D2ModelWidget::setNoPalette()
{
	mUi->penWidthSpinBox->setValue(mWidth);
	mUi->penColorComboBox->setColor(QColor("black"));
}

D2ModelScene *D2ModelWidget::scene()
{
	return mScene;
}

engine::TwoDModelDisplayWidget *D2ModelWidget::display()
{
	return mDisplay;
}

void D2ModelWidget::setSensorVisible(interpreterBase::robotModel::PortInfo const &port, bool isVisible)
{
	qDebug() << "setSensorVisible()";
	/*if (mScene->robot()->sensors()[port]) {
		mScene->robot()->sensors()[port]->setVisible(isVisible);
	}*/
}

void D2ModelWidget::enableRunStopButtons()
{
	mUi->runButton->setEnabled(true);
}

void D2ModelWidget::disableRunStopButtons()
{
	mUi->runButton->setEnabled(false);
}

void D2ModelWidget::closeEvent(QCloseEvent *event)
{
	Q_UNUSED(event)
	serializeParameters();
	emit widgetClosed();
}

SensorItem *D2ModelWidget::sensorItem(interpreterBase::robotModel::PortInfo const &port)
{       	
	qDebug() << "sensorItem()";
	//return mScene->robot()->sensors().value(port);
}

void D2ModelWidget::saveToRepo()
{
	emit mModel.modelChanged(generateXml());
}

QDomDocument D2ModelWidget::generateXml() const
{
	return mModel.serialize();
}

void D2ModelWidget::loadXml(QDomDocument const &worldModel)
{

	mScene->clearScene(true);
	mModel.deserialize(worldModel);

	saveInitialRobotBeforeRun();
}

void D2ModelWidget::setRunStopButtonsEnabled(bool enabled)
{
	mUi->runButton->setEnabled(enabled);
	mUi->stopButton->setEnabled(enabled);
}

void D2ModelWidget::enableRobotFollowing(bool on)
{
	mFollowRobot = on;
	SettingsManager::setValue("2dFollowingRobot", on);
}

void D2ModelWidget::setCursorType(CursorType cursor)
{
	mNoneCursorType = cursor;
	mCursorType = mNoneCursorType;
	SettingsManager::setValue("2dCursorType", cursor);
	mUi->graphicsView->setDragMode(cursorTypeToDragType(cursor));
	mUi->graphicsView->setCursor(cursorTypeToCursor(cursor));
}

void D2ModelWidget::changePhysicsSettings()
{
	SettingsManager::setValue("2DModelRealisticPhysics", mUi->realisticPhysicsCheckBox->isChecked());
	SettingsManager::setValue("enableNoiseOfSensors", mUi->enableSensorNoiseCheckBox->isChecked());
	SettingsManager::setValue("enableNoiseOfMotors", mUi->enableMotorNoiseCheckBox->isChecked());

	mModel.settings().rereadNoiseSettings();
}

void D2ModelWidget::toggleDisplayVisibility()
{
	setDisplayVisibility(!mDisplay->isVisible());
}

void D2ModelWidget::setDisplayVisibility(bool visible)
{
	if (!mDisplay) {
		return;
	}

	mDisplay->setVisible(visible);
	QString const direction = visible ? "right" : "left";
	mUi->displayButton->setIcon(QIcon(QString(":/icons/2d_%1.png").arg(direction)));
	SettingsManager::setValue("2d_displayVisible", visible);
}

QGraphicsView::DragMode D2ModelWidget::cursorTypeToDragType(CursorType type) const
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

QCursor D2ModelWidget::cursorTypeToCursor(CursorType type) const
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

void D2ModelWidget::refreshCursor()
{
	mUi->graphicsView->setDragMode(cursorTypeToDragType(mCursorType));
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));
}

void D2ModelWidget::onHandCursorButtonToggled(bool on)
{
	if (on) {
		setCursorType(hand);
	}
}

void D2ModelWidget::onMultiselectionCursorButtonToggled(bool on)
{
	if (on) {
		setCursorType(multiselection);
	}
}

void D2ModelWidget::syncCursorButtons()
{
	switch(mNoneCursorType) {
	case hand:
		mUi->handCursorButton->setChecked(true);
		break;
	case multiselection:
		mUi->multiselectionCursorButton->setChecked(true);
		break;
	default:
		break;
	}
}

void D2ModelWidget::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	Q_UNUSED(port)
	Q_UNUSED(device)
	qDebug() << "onDeviceConfigurationChanged() in";
	/// @todo Convert configuration between models or something?
	if (mSelectedRobotItem && robotModel == mSelectedRobotItem->robotModel().info()->robotId()) {
		updateWheelComboBoxes();
	}
	qDebug() << "onDeviceConfigurationChanged() out";
}

void D2ModelWidget::initRunStopButtons()
{
	connect(mUi->runButton, &QPushButton::clicked, this, &D2ModelWidget::runButtonPressed);
	connect(mUi->stopButton, &QPushButton::clicked, this, &D2ModelWidget::stopButtonPressed);
}

void D2ModelWidget::updateWheelComboBoxes()
{
	qDebug() << "updateWheelComboBoxes() in";
	if (!mSelectedRobotItem) {
		mUi->leftWheelComboBox->hide();
		mUi->rightWheelComboBox->hide();
		return;
	}

	PortInfo const leftWheelOldPort = mUi->leftWheelComboBox->currentData().value<PortInfo>();
	PortInfo const rightWheelOldPort = mUi->rightWheelComboBox->currentData().value<PortInfo>();

	mUi->leftWheelComboBox->clear();
	mUi->rightWheelComboBox->clear();

	/// @todo More general way of specifying uninitialized values, or someone actually will name some port as "None".
	mUi->leftWheelComboBox->addItem(tr("No wheel"), QVariant::fromValue(PortInfo("None", output)));
	mUi->rightWheelComboBox->addItem(tr("No wheel"), QVariant::fromValue(PortInfo("None", output)));

	for (PortInfo const &port : mSelectedRobotItem->robotModel().info()->availablePorts()) {
		for (DeviceInfo const &device : mSelectedRobotItem->robotModel().info()->allowedDevices(port)) {
			if (device.isA<Motor>()) {
				QString const item = tr("%1 (port %2)").arg(device.friendlyName(), port.name());
				mUi->leftWheelComboBox->addItem(item, QVariant::fromValue(port));
				mUi->rightWheelComboBox->addItem(item, QVariant::fromValue(port));
			}
		}
	}

	auto setSelectedPort = [](QComboBox * const comboBox, PortInfo const &port) {
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
				, mSelectedRobotItem->robotModel().info()->defaultLeftWheelPort())) {

			//qDebug() << "Incorrect defaultLeftWheelPort set in configurer:"
			//		<<  mSelectedRobotItem->robotModel().info();

			if (mUi->leftWheelComboBox->count() > 1) {
				mUi->leftWheelComboBox->setCurrentIndex(1);
			}
		}
	}

	if (!setSelectedPort(mUi->rightWheelComboBox, rightWheelOldPort)) {
		if (!setSelectedPort(mUi->rightWheelComboBox
				, mSelectedRobotItem->robotModel().info()->defaultRightWheelPort())) {

			qDebug() << "Incorrect defaultRightWheelPort set in configurer:"
					<< mSelectedRobotItem->robotModel().info()->defaultRightWheelPort().toString();

			if (mUi->rightWheelComboBox->count() > 2) {
				mUi->rightWheelComboBox->setCurrentIndex(2);
			} else if (mUi->rightWheelComboBox->count() > 1) {
				mUi->rightWheelComboBox->setCurrentIndex(1);
			}
		}
	}
	qDebug() << "updateWheelComboBoxes() out";
}

D2ModelWidget::RobotState::RobotState()
	: pos()
	, rotation(0)
{
}
