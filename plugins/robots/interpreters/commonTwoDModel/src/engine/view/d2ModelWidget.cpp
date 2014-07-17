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

#include "src/engine/model/robotModel.h"
#include "src/engine/model/constants.h"
#include "src/engine/model/timeline.h"

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
	, mRobot(nullptr)
	, mModel(model)
	//, mDisplay(configurer->displayWidget(this))
	, mDrawingAction(none)
	, mMouseClicksCount(0)
	, mCurrentWall(nullptr)
	, mCurrentLine(nullptr)
	, mCurrentStylus(nullptr)
	, mCurrentEllipse(nullptr)
	, mWidth(defaultPenWidth)
	, mClearing(false)
	, mFirstShow(true)
{
	setWindowIcon(QIcon(":/icons/2d-model.svg"));

	initWidget();

	connectUiButtons();

	mUi->realisticPhysicsCheckBox->setChecked(mModel.settings().realisticPhysics());
	mUi->enableSensorNoiseCheckBox->setChecked(mModel.settings().realisticSensors());
	mUi->enableMotorNoiseCheckBox->setChecked(mModel.settings().realisticMotors());
	changePhysicsSettings();

	connect(mScene, &D2ModelScene::mousePressed, this, &D2ModelWidget::mousePressed);
	connect(mScene, &D2ModelScene::mouseMoved, this, &D2ModelWidget::mouseMoved);
	connect(mScene, &D2ModelScene::mouseReleased, this, &D2ModelWidget::mouseReleased);
	connect(mScene, &D2ModelScene::itemDeleted, this, &D2ModelWidget::deleteItem);
	connect(mScene, &D2ModelScene::selectionChanged, this, &D2ModelWidget::onSelectionChange);

	connect(mUi->gridParametersBox, &GridParameters::parametersChanged
			, mScene, &D2ModelScene::updateGrid);
	connect(mUi->gridParametersBox, &GridParameters::parametersChanged
			, this, &D2ModelWidget::alignWalls);

	connect(&mModel.timeline(), &Timeline::started, [this]() { mUi->timelineBox->setValue(0); });
	connect(&mModel.timeline(), &Timeline::started, mDisplay, &engine::TwoDModelDisplayWidget::clear);
	connect(&mModel.timeline(), &Timeline::tick, [this]() { mUi->timelineBox->stepBy(1); });

	connect(&mModel, twoDModel::model::Model::robotModelsListChanged(), this, onRobotModelListChange());

	/*connect(&mModel.robotModel(), &RobotModel::positionChanged, this, &D2ModelWidget::centerOnRobot);
	connect(&mModel.robotModel().configuration(), &SensorsConfiguration::deviceAdded
			, this, &D2ModelWidget::reinitSensor);*/

	setCursorType(static_cast<CursorType>(SettingsManager::value("2dCursorType").toInt()));
	syncCursorButtons();
	enableRobotFollowing(SettingsManager::value("2dFollowingRobot").toBool());
	mUi->autoCenteringButton->setChecked(mFollowRobot);
	mUi->noneButton->setChecked(true);

	/*auto connectWheelComboBox = [this](QComboBox * const comboBox, RobotModel::WheelEnum wheel) {
			connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
					, [this, wheel, comboBox](int index) {
							mModel.robotModel().setMotorPortOnWheel(wheel, comboBox->itemData(index).value<PortInfo>());
					});
	};*/

	//connectWheelComboBox(mUi->leftWheelComboBox, RobotModel::left);
	//connectWheelComboBox(mUi->rightWheelComboBox, RobotModel::right);

	drawInitialRobot();

	setFocus();

	mUi->timelineBox->setSingleStep(Timeline::timeInterval * 0.001);
}

D2ModelWidget::~D2ModelWidget()
{
	delete mRobot;
	delete mScene;
	delete mUi;
}

void D2ModelWidget::initWidget()
{
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);

	mUi->setupUi(this);

	mScene = new D2ModelScene(mUi->graphicsView);
	mUi->graphicsView->setScene(mScene);
	move(0, 0);

	mUi->penWidthSpinBox->setRange(1, 30);

	QStringList colorList = QStringList()
			<< "Black"
			<< "Blue"
			<< "Green"
			<< "Yellow"
			<< "Red";

	QStringList translatedColorList = QStringList()
			<< tr("Black")
			<< tr("Blue")
			<< tr("Green")
			<< tr("Yellow")
			<< tr("Red");

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

	/*if (mModel.robotModels().size() == 1) {
		mSelectedRobotItem=
	}*/
}

void D2ModelWidget::connectUiButtons()
{
	connect(mUi->realisticPhysicsCheckBox, SIGNAL(toggled(bool)), this, SLOT(changePhysicsSettings()));
	connect(mUi->enableMotorNoiseCheckBox, SIGNAL(toggled(bool)), this, SLOT(changePhysicsSettings()));
	connect(mUi->enableSensorNoiseCheckBox, SIGNAL(toggled(bool)), this, SLOT(changePhysicsSettings()));

	connect(mUi->ellipseButton, SIGNAL(toggled(bool)), this, SLOT(addEllipse(bool)));
	connect(mUi->stylusButton, SIGNAL(toggled(bool)), this, SLOT(addStylus(bool)));
	connect(mUi->lineButton, SIGNAL(toggled(bool)), this, SLOT(addLine(bool)));
	connect(mUi->wallButton, SIGNAL(toggled(bool)), this, SLOT(addWall(bool)));
	connect(mUi->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));
	connect(mUi->noneButton, SIGNAL(clicked()), this, SLOT(setNoneButton()));

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
	mCurrentConfigurer = new DevicesConfigurationWidget(mUi->portsGroupBox, true, true);
	mCurrentConfigurer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	mCurrentConfigurer->loadRobotModels({ &mSelectedRobotItem->robotModel().info() });
	mCurrentConfigurer->selectRobotModel(mSelectedRobotItem->robotModel().info());
	mUi->portsGroupBox->layout()->addWidget(mCurrentConfigurer);
	mCurrentConfigurer->connectDevicesConfigurationProvider(&mSelectedRobotItem.robotModel().configuration());
	connectDevicesConfigurationProvider(&mSelectedRobotItem.robotModel().configuration());

	auto connectWheelComboBox = [this](QComboBox * const comboBox, RobotModel::WheelEnum wheel) {
				connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
						, [this, wheel, comboBox](int index) {
								mSelectedRobotItem.robotModel().setMotorPortOnWheel(wheel
										, comboBox->itemData(index).value<PortInfo>());
						});
		};

	connectWheelComboBox(mUi->leftWheelComboBox, RobotModel::left);
	connectWheelComboBox(mUi->rightWheelComboBox, RobotModel::right);
}

void D2ModelWidget::unsetPortsGroupBoxAndWheelComboBoxes()
{
	if (mCurrentConfigurer) {
		mUi->portsGroupBox->layout()->removeWidget(mCurrentConfigurer);

		if (mSelectedRobotItem) {
			mSelectedRobotItem->robotModel().configuration().disconnectDevicesConfigurationProvider();
		}

		delete mCurrentConfigurer;
	}
}

void D2ModelWidget::setHighlightOneButton(QAbstractButton * const oneButton)
{
	foreach (QAbstractButton * const button, mButtonGroup.buttons()) {
		if (button != oneButton) {
			button->setChecked(false);
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
	/*if (!mRobot) {
		drawInitialRobot();
	}*/

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
	//mInitialRobotBeforeRun.pos = mModel.robotModel().position();
	//mInitialRobotBeforeRun.rotation = mModel.robotModel().rotation();
}

void D2ModelWidget::setInitialRobotBeforeRun()
{
	//mModel.robotModel().setPosition(mInitialRobotBeforeRun.pos);
	//mModel.robotModel().setRotation(mInitialRobotBeforeRun.rotation);
}

void D2ModelWidget::drawInitialRobot()
{
	/*mRobot = new RobotItem(mConfigurer->robotImage(), mModel.robotModel());
	connect(mRobot, SIGNAL(changedPosition()), this, SLOT(handleNewRobotPosition()));
	connect(mRobot, SIGNAL(mousePressed()), this, SLOT(setNoneButton()));
	mScene->addItem(mRobot);

	Rotater * const rotater = new Rotater();
	rotater->setMasterItem(mRobot);
	rotater->setVisible(false);

	mRobot->setRotater(rotater);

	mUi->graphicsView->centerOn(mRobot);*/
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

QPointF D2ModelWidget::robotPos() const
{
	return mRobot ? mRobot->pos() : QPointF(0, 0);
}

void D2ModelWidget::close()
{
	setVisible(false);
}

void D2ModelWidget::update()
{
	QWidget::update();
	drawWalls();
	drawColorFields();
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
	/*if (mFollowRobot && mModel.robotModel().onTheGround()) {
		QRectF const viewPortRect = mUi->graphicsView->mapToScene(mUi->graphicsView->viewport()->rect()).boundingRect();
		if (!viewPortRect.contains(mRobot->sceneBoundingRect().toRect())) {
			QRectF const requiredViewPort = viewPortRect.translated(mRobot->scenePos() - viewPortRect.center());
			mScene->setSceneRect(mScene->itemsBoundingRect().united(requiredViewPort));
			mUi->graphicsView->centerOn(mRobot);
		}
	}*/
}

void D2ModelWidget::drawWalls()
{
	if (mDrawingAction == wall || mDrawingAction == noneWordLoad) {
		for (items::WallItem *wall : mModel.worldModel().walls()) {
			if (!mScene->items().contains(wall)) {
				mScene->addItem(wall);
				connect(wall, &items::WallItem::wallDragged, this, &D2ModelWidget::worldWallDragged);
			}
		}
	}
}

void D2ModelWidget::drawColorFields()
{
	if (mDrawingAction == line
			|| mDrawingAction == stylus
			|| mDrawingAction == ellipse
			|| mDrawingAction == noneWordLoad) {
		for (items::ColorFieldItem *colorField : mModel.worldModel().colorFields()) {
			if (!mScene->items().contains(colorField)) {
				mScene->addItem(colorField);
			}
		}
	}
}

void D2ModelWidget::addWall(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->wallButton);
	setCursorTypeForDrawing(drawWall);
	mDrawingAction = wall;
}

void D2ModelWidget::addLine(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->lineButton);
	setCursorTypeForDrawing(drawLine);
	mDrawingAction = line;
}

void D2ModelWidget::addStylus(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->stylusButton);
	setCursorTypeForDrawing(drawStylus);
	mDrawingAction = stylus;
}

void D2ModelWidget::addEllipse(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->ellipseButton);
	setCursorTypeForDrawing(drawEllipse);
	mDrawingAction = ellipse;
}

void D2ModelWidget::setNoneButton()
{
	setHighlightOneButton(mUi->noneButton);
	setNoneStatus();
}

void D2ModelWidget::setNoneStatus()
{
	mDrawingAction = none;
	mMouseClicksCount = 0;
	setCursorTypeForDrawing(mNoneCursorType);
	mUi->noneButton->setChecked(true);
}

void D2ModelWidget::setCursorTypeForDrawing(CursorType type)
{
	mCursorType = type;
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));
}

void D2ModelWidget::clearScene(bool removeRobot)
{
	mClearing = true;
	mModel.worldModel().clearScene();
	//mModel.robotModel().clear();
	if (removeRobot) {
		/*for (PortInfo const &port : mRobot->sensors().keys()) {
			deviceConfigurationChanged(mModel.robotModel().info().name(), port, DeviceInfo());
		}*/

		delete mRobot;
		mScene->clear();
		drawInitialRobot();
	} else {
		/*for (QGraphicsItem * const item : mScene->items()) {
			if (item != mRobot && !mRobot->isAncestorOf(item)) {
				mScene->removeItem(item);
				delete item;
			}
		}*/
	}

	mClearing = false;
}

void D2ModelWidget::resetButtons()
{
	mCurrentWall = nullptr;
	mCurrentLine = nullptr;
	mCurrentStylus = nullptr;
	mMouseClicksCount = 0;
	mDrawingAction = none;
}

void D2ModelWidget::reshapeWall(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentWall) {
		QPointF const oldPos = mCurrentWall->end();
		mCurrentWall->setX2andY2(pos.x(), pos.y());
		if (SettingsManager::value("2dShowGrid").toBool()) {
			mCurrentWall->reshapeBeginWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			mCurrentWall->reshapeEndWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		} else {
			/*if (mCurrentWall->realShape().intersects(mRobot->realBoundingRect())) {
				mCurrentWall->setX2andY2(oldPos.x(), oldPos.y());
			}*/
			if (event->modifiers() & Qt::ShiftModifier) {
				mCurrentWall->reshapeRectWithShift();
			}
		}
	}
}

void D2ModelWidget::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentLine) {
		mCurrentLine->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentLine->reshapeRectWithShift();
		}
	}
}

void D2ModelWidget::reshapeStylus(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentStylus) {
		mCurrentStylus->addLine(pos.x(), pos.y());
	}
}

void D2ModelWidget::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentEllipse) {
		mCurrentEllipse->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentEllipse->reshapeRectWithShift();
		}
	}
}

void D2ModelWidget::mousePressed(QGraphicsSceneMouseEvent *mouseEvent)
{
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));

	/*mRobot->checkSelection();
	for (SensorItem *sensor : mRobot->sensors().values()) {
		if (sensor) {
			sensor->checkSelection();
		}
	}*/

	QPointF const position = mouseEvent->scenePos();
	processDragMode();

	switch (mDrawingAction) {
	case wall: {
		/*if (!mRobot->realBoundingRect().intersects(QRectF(position, position))) {
			mCurrentWall = new items::WallItem(position, position);
			mScene->removeMoveFlag(mouseEvent, mCurrentWall);
			mModel.worldModel().addWall(mCurrentWall);
			mMouseClicksCount++;
		}*/
		break;
	}
	case line: {
		mCurrentLine = new items::LineItem(position, position);
		mCurrentLine->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
				, mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentLine);
		mModel.worldModel().addColorField(mCurrentLine);
		mMouseClicksCount++;
		break;
	}
	case stylus: {
		mCurrentStylus = new items::StylusItem(position.x(), position.y());
		mCurrentStylus->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
				, mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentStylus);
		mModel.worldModel().addColorField(mCurrentStylus);
		mMouseClicksCount++;
		break;
	}
	case ellipse: {
		mCurrentEllipse = new items::EllipseItem(position, position);
		mCurrentEllipse->setPen(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentEllipse);
		mModel.worldModel().addColorField(mCurrentEllipse);
		mMouseClicksCount++;
		break;
	}
	case none: {
		mMouseClicksCount = 0;
		mScene->forPressResize(mouseEvent);
		break;
	}
	default:
		break;
	}

	update();
}

void D2ModelWidget::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->buttons() & Qt::LeftButton) {
		/*mRobot->checkSelection();
		for (SensorItem *sensor : mRobot->sensors().values()) {
			if (sensor) {
				sensor->checkSelection();
			}
		}*/
	}

	bool needUpdate = true;
	processDragMode();
	switch (mDrawingAction){
	case wall:
		reshapeWall(mouseEvent);
		break;
	case line:
		reshapeLine(mouseEvent);
		break;
	case stylus:
		reshapeStylus(mouseEvent);
		break;
	case ellipse:
		reshapeEllipse(mouseEvent);
		break;
	default:
		needUpdate = false;
		if (mouseEvent->buttons() & Qt::LeftButton) {
			//mScene->forMoveResize(mouseEvent, mRobot->realBoundingRect());
		}
		break;
	}

	if (needUpdate) {
		mScene->update();
	}
}

void D2ModelWidget::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));

	/*mRobot->checkSelection();
	for (SensorItem *sensor : mRobot->sensors().values()) {
		if (sensor) {
			sensor->checkSelection();
		}
	}*/

	processDragMode();

	switch (mDrawingAction){
	case wall: {
		reshapeWall(mouseEvent);
		mCurrentWall->setSelected(true);
		mCurrentWall = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case line: {
		reshapeLine(mouseEvent);
		mCurrentLine->setSelected(true);
		mCurrentLine = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case stylus: {
		reshapeStylus(mouseEvent);
		mCurrentStylus->setSelected(true);
		mCurrentStylus = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case ellipse: {
		reshapeEllipse(mouseEvent);
		mCurrentEllipse->setSelected(true);
		mCurrentEllipse = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	default:
		//mScene->forReleaseResize(mouseEvent, mRobot->realBoundingRect());
		break;
	}

	mScene->setMoveFlag(mouseEvent);

	mScene->update();
	saveToRepo();
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
	// Loads world and robot models simultaneously.
	QString const loadFileName = QRealFileDialog::getOpenFileName("Open2DModelWidget", this
			, tr("Loading world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (loadFileName.isEmpty()) {
		return;
	}

	clearScene(true);

	QDomDocument const save = utils::xmlUtils::loadDocument(loadFileName);
	loadXml(save);
}

void D2ModelWidget::handleNewRobotPosition()
{
	/*for (items::WallItem const *wall : mModel.worldModel().walls()) {
		if (wall->realShape().intersects(mRobot->realBoundingRect())) {
			mRobot->recoverDragStartPosition();
			return;
		}
	}*/
}

void D2ModelWidget::reinitSensor(PortInfo const &port)
{
	/*mRobot->removeSensor(port);

	DeviceInfo const &device = mModel.robotModel().configuration().type(port);
	if (device.isNull() || (
			/// @todo: Add supported by 2D model sensors here
			!device.isA<TouchSensor>()
			&& !device.isA<ColorSensor>()
			&& !device.isA<LightSensor>()
			&& !device.isA<RangeSensor>()
			))
	{
		return;
	}

	SensorItem *sensor = device.isA<RangeSensor>()
			? new SonarSensorItem(mModel.worldModel(), mModel.robotModel().configuration()
					, port
					, mConfigurer->sensorImagePath(device)
					, mConfigurer->sensorImageRect(device)
					)
			: new SensorItem(mModel.robotModel().configuration()
					, port
					, mConfigurer->sensorImagePath(device)
					, mConfigurer->sensorImageRect(device)
					);

	mRobot->addSensor(port, sensor);*/
}

void D2ModelWidget::deleteItem(QGraphicsItem *item)
{
	if (!scene()->items().contains(item)) {
		return;
	}

	/// @todo Handle all cases equally
	if (SensorItem * const sensor = dynamic_cast<SensorItem *>(item)) {
		/*PortInfo const port = mRobot->sensors().key(sensor);
		if (port.isValid()) {
			deviceConfigurationChanged(mModel.robotModel().info().name(), port, DeviceInfo());
		}*/

		return;
	}

	if (items::WallItem * const wall = dynamic_cast<items::WallItem *>(item)) {
		mScene->removeItem(wall);
		mModel.worldModel().removeWall(wall);
		return;
	}

	if (items::ColorFieldItem *colorField = dynamic_cast<items::ColorFieldItem *>(item)) {
		mScene->removeItem(colorField);
		mModel.worldModel().removeColorField(colorField);
		delete colorField;
	}
}

bool D2ModelWidget::isColorItem(AbstractItem *item)
{
	RobotItem *robotItem = dynamic_cast<RobotItem *>(item);
	SensorItem *sensorItem = dynamic_cast<SensorItem *>(item);
	items::WallItem *wallItem = dynamic_cast<items::WallItem *>(item);
	Rotater *rotaterItem = dynamic_cast<Rotater *>(item);
	return (!robotItem && !sensorItem && !wallItem && !rotaterItem);
}

QList<AbstractItem *> D2ModelWidget::selectedColorItems()
{
	QList<AbstractItem *> resList;
	QList<QGraphicsItem *> listSelectedItems = mScene->selectedItems();
	foreach (QGraphicsItem * const graphicsItem, listSelectedItems) {
		AbstractItem* item = dynamic_cast<AbstractItem*>(graphicsItem);
		// excluding objects  with immutable color (robot, sensors, walls and rotators)
		if (item && isColorItem(item)) {
			resList.push_back(item);
		}
	}

	qSort(resList.begin(), resList.end(), mScene->compareItems);
	return resList;
}

void D2ModelWidget::changePenWidth(int width)
{
	mScene->setPenWidthItems(width);
	mWidth = width;
	foreach (AbstractItem *item, selectedColorItems()) {
		item->setPenWidth(width);
	}

	mScene->update();
}

void D2ModelWidget::changePenColor(int textIndex)
{
	QString text = mUi->penColorComboBox->colorByIndex(textIndex).name();
	mScene->setPenColorItems(text);
	foreach (AbstractItem *item, selectedColorItems()) {
		item->setPenColor(text);
	}

	mScene->update();
}

void D2ModelWidget::changePalette()
{
	if (mClearing) {
		return;
	}

	if(mDrawingAction == none) {
		QList<QGraphicsItem *> listSelectedItems = mScene->selectedItems();
		if (listSelectedItems.isEmpty()) {
			setNoPalette();
			mScene->setEmptyPenBrushItems();
		} else {
			AbstractItem* item = dynamic_cast<AbstractItem*>(listSelectedItems.back());
			if (isColorItem(item)) {
				QPen const penItem = item->pen();
				QBrush const brushItem = item->brush();
				setItemPalette(penItem, brushItem);
				mScene->setPenBrushItems(penItem, brushItem);
			}
		}
	}
}

void D2ModelWidget::onSelectionChange()
{
	changePalette();

	QList<QGraphicsItem *> listSelectedItems = mScene->selectedItems();
	RobotItem *robotItem;
	bool oneRobotItem = false;

	for (QGraphicsItem const *item : listSelectedItems) {
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
				&& robotItem->robotModel().info().name() == mSelectedRobotItem->robotModel().info().name()) {
			return;
		}

		if (mSelectedRobotItem) {
			unsetPortsGroupBoxAndWheelComboBoxes();
			mSelectedRobotItem = nullptr;
		}

		mSelectedRobotItem = robotItem;

		setPortsGroupBoxAndWheelComboBoxes();
		updateWheelComboBoxes();
		mUi->leftWheelComboBox->show();
		mUi->rightWheelComboBox->show();
	}
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
	/*if (mRobot->sensors()[port]) {
		mRobot->sensors()[port]->setVisible(isVisible);
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
	//return mRobot->sensors().value(port);
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
	clearScene(true);

	mModel.deserialize(worldModel);

	mDrawingAction = noneWordLoad;
	update();
	mDrawingAction = none;

	saveInitialRobotBeforeRun();
}

void D2ModelWidget::setRunStopButtonsEnabled(bool enabled)
{
	mUi->runButton->setEnabled(enabled);
	mUi->stopButton->setEnabled(enabled);
}

void D2ModelWidget::worldWallDragged(items::WallItem *wall, QPainterPath const &shape, QPointF const &oldPos)
{
	/*bool const isNeedStop = shape.intersects(mRobot->realBoundingRect());
	wall->onOverlappedWithRobot(isNeedStop);
	if (wall->isDragged() && ((mDrawingAction == none) ||
			(mDrawingAction == D2ModelWidget::wall && mCurrentWall == wall)))
	{
		wall->setFlag(QGraphicsItem::ItemIsMovable, !isNeedStop);
		if (isNeedStop) {
			wall->setPos(oldPos);
		}
	}*/
}

void D2ModelWidget::enableRobotFollowing(bool on)
{
	mFollowRobot = on;
	qReal::SettingsManager::setValue("2dFollowingRobot", on);
}

void D2ModelWidget::setCursorType(CursorType cursor)
{
	mNoneCursorType = cursor;
	mCursorType = mNoneCursorType;
	qReal::SettingsManager::setValue("2dCursorType", cursor);
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

void D2ModelWidget::processDragMode()
{
	mUi->graphicsView->setDragMode(cursorTypeToDragType(mCursorType));
}

void D2ModelWidget::onHandCursorButtonToggled(bool on)
{
	if (on) {
		setHighlightOneButton(mUi->handCursorButton);
		setCursorType(hand);
	}
}

void D2ModelWidget::onMultiselectionCursorButtonToggled(bool on)
{
	if (on) {
		setHighlightOneButton(mUi->multiselectionCursorButton);
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

void D2ModelWidget::alignWalls()
{
	for (items::WallItem * const wall : mModel.worldModel().walls()) {
		if (mScene->items().contains(wall)) {
			wall->setBeginCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			wall->setEndCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		}
	}
}

void D2ModelWidget::onDeviceConfigurationChanged(QString const &robotModel
		, PortInfo const &port, DeviceInfo const &device)
{
	Q_UNUSED(port)
	Q_UNUSED(device)
	/// @todo Convert configuration between models or something?
	/*if (mRobot && robotModel == mModel.robotModel().info().name()) {
		updateWheelComboBoxes();
	}*/
}

void D2ModelWidget::initRunStopButtons()
{
	connect(mUi->runButton, &QPushButton::clicked, this, &D2ModelWidget::runButtonPressed);
	connect(mUi->stopButton, &QPushButton::clicked, this, &D2ModelWidget::stopButtonPressed);
}

void D2ModelWidget::updateWheelComboBoxes()
{
	if (!mSelectedRobotItem) {
		mUi->leftWheelComboBox->hide();
		mUi->rightWheelComboBox->hide();
	}

	PortInfo const leftWheelOldPort = mUi->leftWheelComboBox->currentData().value<PortInfo>();
	PortInfo const rightWheelOldPort = mUi->rightWheelComboBox->currentData().value<PortInfo>();

	mUi->leftWheelComboBox->clear();
	mUi->rightWheelComboBox->clear();

	/// @todo More general way of specifying uninitialized values, or someone actually will name some port as "None".
	mUi->leftWheelComboBox->addItem(tr("No wheel"), QVariant::fromValue(PortInfo("None", output)));
	mUi->rightWheelComboBox->addItem(tr("No wheel"), QVariant::fromValue(PortInfo("None", output)));

	for (PortInfo const &port : mSelectedRobotItem->robotModel().info().availablePorts()) {
		for (DeviceInfo const &device : mSelectedRobotItem->robotModel().info().allowedDevices(port)) {
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
				, mSelectedRobotItem->robotModel().info().defaultLeftWheelPort())) {

			qDebug() << "Incorrect defaultLeftWheelPort set in configurer:"
					<<  mSelectedRobotItem->robotModel().info().toString();

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

D2ModelWidget::RobotState::RobotState()
	: pos()
	, rotation(0)
{
}
