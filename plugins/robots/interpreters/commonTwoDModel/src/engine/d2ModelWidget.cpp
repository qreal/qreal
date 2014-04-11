#include "d2ModelWidget.h"
#include "ui_d2Form.h"

#include <QtCore/qmath.h>
#include <QtGui/QRegion>

#include <qrkernel/settingsManager.h>
#include <qrutils/outFile.h>
#include <qrutils/xmlUtils.h>
#include <qrutils/qRealFileDialog.h>

#include <interpreterBase/robotModel/robotParts/motor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>

#include "d2RobotModel.h"
#include "constants.h"
#include "sensorItem.h"
#include "sonarSensorItem.h"
#include "rotater.h"
#include "timeline.h"

using namespace twoDModel;
using namespace qReal;
using namespace utils;
using namespace graphicsUtils;
using namespace interpreterBase::robotModel;
using namespace interpreterBase::robotModel::robotParts;

D2ModelWidget::D2ModelWidget(TwoDRobotRobotModelInterface *twoDRobotModel, WorldModel *worldModel
		, RobotModelInterface &robotModel
		/* , NxtDisplay *nxtDisplay*/, QWidget *parent)
	: QRealDialog("D2ModelWindow", parent)
//	, details::SensorsConfigurationProvider("D2ModelWidget")
	, mUi(new Ui::D2Form)
	, mScene(nullptr)
	, mRobot(nullptr)
	, mMaxDrawCyclesBetweenPathElements(SettingsManager::value("drawCyclesBetweenPathElements").toInt())
	, mTwoDRobotModel(twoDRobotModel)
	, mWorldModel(worldModel)
//	, mNxtDisplay(nxtDisplay)
	, mDrawingAction(enums::drawingAction::none)
	, mMouseClicksCount(0)
	, mCurrentWall(nullptr)
	, mCurrentLine(nullptr)
	, mCurrentStylus(nullptr)
	, mCurrentEllipse(nullptr)
	, mWidth(defaultPenWidth)
	, mClearing(false)
	, mFirstShow(true)
	, mTimeline(dynamic_cast<D2RobotModel *>(twoDRobotModel)->timeline())
	, mRobotModel(robotModel)
{
	setWindowIcon(QIcon(":/icons/2d-model.svg"));

	initWidget();

	connectUiButtons();

	mUi->realisticPhysicsCheckBox->setChecked(SettingsManager::value("2DModelRealisticPhysics").toBool());
	mUi->enableSensorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfSensors").toBool());
	mUi->enableMotorNoiseCheckBox->setChecked(SettingsManager::value("enableNoiseOfMotors").toBool());
	changePhysicsSettings();

	connect(mScene, SIGNAL(mousePressed(QGraphicsSceneMouseEvent *)), this, SLOT(mousePressed(QGraphicsSceneMouseEvent*)));
	connect(mScene, SIGNAL(mouseMoved(QGraphicsSceneMouseEvent*)), this, SLOT(mouseMoved(QGraphicsSceneMouseEvent*)));
	connect(mScene, SIGNAL(mouseReleased(QGraphicsSceneMouseEvent*)), this, SLOT(mouseReleased(QGraphicsSceneMouseEvent*)));
	connect(mScene, SIGNAL(itemDeleted(QGraphicsItem*)), this, SLOT(deleteItem(QGraphicsItem*)));

	connect(mScene, SIGNAL(selectionChanged()), this, SLOT(changePalette()));

	connect(mUi->gridParametersBox, SIGNAL(parametersChanged()), mScene, SLOT(updateGrid()));
	connect(mUi->gridParametersBox, SIGNAL(parametersChanged()), this, SLOT(alignWalls()));

	setCursorType(static_cast<enums::cursorType::CursorType>(SettingsManager::value("2dCursorType").toInt()));
	syncCursorButtons();
	enableRobotFollowing(SettingsManager::value("2dFollowingRobot").toBool());
	mUi->autoCenteringButton->setChecked(mFollowRobot);
	mUi->noneButton->setChecked(true);

	auto connectWheelComboBox = [this](QComboBox * const comboBox, TwoDRobotRobotModelInterface::WheelEnum wheel) {
			connect(comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated)
					, [this, wheel, comboBox](int index) {
							mTwoDRobotModel->setMotorPortOnWheel(
									wheel
									, comboBox->itemData(index).value<PortInfo>()
									);
					});
	};

	connectWheelComboBox(mUi->leftWheelComboBox, TwoDRobotRobotModelInterface::left);
	connectWheelComboBox(mUi->rightWheelComboBox, TwoDRobotRobotModelInterface::right);

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

//	mNxtDisplay->setMinimumSize(displaySize);
//	mNxtDisplay->setMaximumSize(displaySize);
//	dynamic_cast<QHBoxLayout *>(mUi->displayFrame->layout())->insertWidget(0, mNxtDisplay);
	setDisplayVisibility(SettingsManager::value("2d_displayVisible").toBool());

	initPorts();
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

//	connect(&mPortsMapper, SIGNAL(mapped(int)), this, SLOT(addPort(int)));
//	connect(&mPortsMapper, SIGNAL(mapped(int)), this, SLOT(saveToRepo()));

//	connect(mUi->port1Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
//	mPortsMapper.setMapping(mUi->port1Box, robots::enums::inputPort::port1);
//	connect(mUi->port2Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
//	mPortsMapper.setMapping(mUi->port2Box, robots::enums::inputPort::port2);
//	connect(mUi->port3Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
//	mPortsMapper.setMapping(mUi->port3Box, robots::enums::inputPort::port3);
//	connect(mUi->port4Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
//	mPortsMapper.setMapping(mUi->port4Box, robots::enums::inputPort::port4);

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

void D2ModelWidget::initPorts()
{
	QList<PortInfo> ports = mRobotModel.configurablePorts();
	qSort(ports.begin(), ports.end()
			, [](PortInfo const &port1, PortInfo const &port2) { return port1.name() < port2.name(); }
	);

	for (PortInfo const &port : ports) {
		DeviceInfo const currentlyConfiguredDevice = currentConfiguration(mRobotModel.name(), port);

		mUi->portsGroupBox->layout()->addWidget(new QLabel(tr("Port ") + port.name()));

		QComboBox *portsSelectionComboBox = new QComboBox();
		mUi->portsGroupBox->layout()->addWidget(portsSelectionComboBox);

		portsSelectionComboBox->addItem(tr("none"));
		for (DeviceInfo device : mRobotModel.allowedDevices(port)) {
			portsSelectionComboBox->addItem(device.friendlyName(), QVariant::fromValue(device));

			if (currentlyConfiguredDevice == device) {
				portsSelectionComboBox->setCurrentIndex(portsSelectionComboBox->count() - 1);
			}
		}

		connect(portsSelectionComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated)
				, this, &D2ModelWidget::addPort);

		mComboBoxesToPortsMap.insert(portsSelectionComboBox, port);
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
		mTwoDRobotModel->setSpeedFactor(Timeline::slowSpeedFactor);
		break;
	case 1:
		mTwoDRobotModel->setSpeedFactor(Timeline::normalSpeedFactor);
		break;
	case 2:
		mTwoDRobotModel->setSpeedFactor(Timeline::fastSpeedFactor);
		break;
	default:
		mTwoDRobotModel->setSpeedFactor(Timeline::normalSpeedFactor);
	}
}

void D2ModelWidget::init(bool isActive)
{
	if (!isActive) {
		hide();
		return;
	}

	if (!mRobot) {
		drawInitialRobot();
	}

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
	mInitialRobotBeforeRun.pos = mRobot->robotPos();
	mInitialRobotBeforeRun.rotation = mRobot->rotateAngle();
}

void D2ModelWidget::setInitialRobotBeforeRun()
{
	mRobot->setRobotPos(mInitialRobotBeforeRun.pos);
	mRobot->setRotateAngle(mInitialRobotBeforeRun.rotation);
	mScene->update();
}

void D2ModelWidget::drawInitialRobot()
{
	mRobot = new RobotItem();
	connect(mRobot, SIGNAL(changedPosition()), this, SLOT(handleNewRobotPosition()));
	connect(mRobot, SIGNAL(mousePressed()), this, SLOT(setNoneButton()));
	mScene->addItem(mRobot);

	Rotater * const rotater = new Rotater();
	rotater->setMasterItem(mRobot);
	rotater->setVisible(false);

	mRobot->setRotater(rotater);
	mRobot->setRobotModel(mTwoDRobotModel);

	mUi->graphicsView->centerOn(mRobot);
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
	mClearing = true;
	if (mRobot) {
		disconnect(this, SLOT(changePalette()));
		mRobot->resetTransform();
		mScene->clear();
		mRobot = NULL;
	}

	mUi->graphicsView->setVisible(false);
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
//	refreshSensorsConfiguration();
}

bool D2ModelWidget::isRobotOnTheGround()
{
	return mRobot && mRobot->isOnTheGround();
}

void D2ModelWidget::draw(QPointF const &newCoord, qreal angle)
{
	mRobot->setPos(newCoord);
	mRobot->setRotation(angle);

	centerOnRobot();
}

void D2ModelWidget::centerOnRobot()
{
	if (mFollowRobot) {
		QRectF const viewPortRect = mUi->graphicsView->mapToScene(mUi->graphicsView->viewport()->rect()).boundingRect();
		if (!viewPortRect.contains(mRobot->sceneBoundingRect().toRect())) {
			QRectF const requiredViewPort = viewPortRect.translated(mRobot->scenePos() - viewPortRect.center());
			mScene->setSceneRect(mScene->itemsBoundingRect().united(requiredViewPort));
			mUi->graphicsView->centerOn(mRobot);
		}
	}
}

void D2ModelWidget::drawWalls()
{
	if (mDrawingAction == enums::drawingAction::wall || mDrawingAction == enums::drawingAction::noneWordLoad) {
		foreach (WallItem *wall, mWorldModel->walls()) {
			if (!mScene->items().contains(wall)) {
				mScene->addItem(wall);
				connect(wall, SIGNAL(wallDragged(WallItem*,QPainterPath,QPointF))
						, this, SLOT(worldWallDragged(WallItem*,QPainterPath,QPointF)));
			}
		}
	}
}

void D2ModelWidget::drawColorFields()
{
	if (mDrawingAction == enums::drawingAction::line
			|| mDrawingAction == enums::drawingAction::stylus
			|| mDrawingAction == enums::drawingAction::ellipse
			|| mDrawingAction == enums::drawingAction::noneWordLoad) {
		foreach (ColorFieldItem *colorField, mWorldModel->colorFields()) {
			if (!mScene->items().contains(colorField)) {
				mScene->addItem(colorField);
			}
		}
	}
}

void D2ModelWidget::drawBeep(bool isNeededBeep)
{
	mRobot->setNeededBeep(isNeededBeep);
}

QPainterPath const D2ModelWidget::robotBoundingPolygon(QPointF const &coord
		, qreal const &angle) const
{
	QPainterPath path;
	path.addRect(mRobot->boundingRect());
	mRobot->addSensorsShapes(path);
	QPointF const realRotatePoint = QPointF(mRobot->boundingRect().width() / 2, mRobot->boundingRect().height() / 2);
	QPointF const translationToZero = -realRotatePoint - mRobot->boundingRect().topLeft();
	QPointF const finalTranslation = coord + realRotatePoint + mRobot->boundingRect().topLeft();
	QTransform const transform = QTransform().translate(finalTranslation.x(), finalTranslation.y())
			.rotate(angle).translate(translationToZero.x(), translationToZero.y());
	return transform.map(path);
}

void D2ModelWidget::addWall(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->wallButton);
	setCursorTypeForDrawing(enums::cursorType::drawWall);
	mDrawingAction = enums::drawingAction::wall;
}

void D2ModelWidget::addLine(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->lineButton);
	setCursorTypeForDrawing(enums::cursorType::drawLine);
	mDrawingAction = enums::drawingAction::line;
}

void D2ModelWidget::addStylus(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->stylusButton);
	setCursorTypeForDrawing(enums::cursorType::drawStylus);
	mDrawingAction = enums::drawingAction::stylus;
}

void D2ModelWidget::addEllipse(bool on)
{
	if (!on) {
		setNoneStatus();
		return;
	}

	setHighlightOneButton(mUi->ellipseButton);
	setCursorTypeForDrawing(enums::cursorType::drawEllipse);
	mDrawingAction = enums::drawingAction::ellipse;
}

void D2ModelWidget::setNoneButton()
{
	setHighlightOneButton(mUi->noneButton);
	setNoneStatus();
}

void D2ModelWidget::setNoneStatus()
{
	mDrawingAction = enums::drawingAction::none;
	mMouseClicksCount = 0;
	setCursorTypeForDrawing(mNoneCursorType);
}

void D2ModelWidget::setCursorTypeForDrawing(enums::cursorType::CursorType type)
{
	mCursorType = type;
	mUi->graphicsView->setCursor(cursorTypeToCursor(mCursorType));
}

void D2ModelWidget::clearScene(bool removeRobot)
{
	mClearing = true;
	mWorldModel->clearScene();
	mTwoDRobotModel->clear();
	if (removeRobot) {
		for (PortInfo const &port : mSensors.keys()) {
			removeSensor(port);
		}

		clearConfiguration();  // Well...
		mScene->clear();
		drawInitialRobot();
	} else {
		for (QGraphicsItem * const item : mScene->items()) {
			if (!dynamic_cast<RobotItem *>(item)
					&& !dynamic_cast<SensorItem *>(item)
					&& !dynamic_cast<SensorItem::PortItem *>(item)
					&& !dynamic_cast<Rotater *>(item)
					&& !dynamic_cast<BeepItem *>(item))
			{
				mScene->removeItem(item);
				delete item;
			}
		}
	}

	mClearing = false;
}

void D2ModelWidget::resetButtons()
{
	mCurrentWall = nullptr;
	mCurrentLine = nullptr;
	mCurrentStylus = nullptr;
	mMouseClicksCount = 0;
	mDrawingAction = enums::drawingAction::none;
}

QComboBox *D2ModelWidget::currentComboBox()
{
//	switch (mCurrentPort) {
//	case robots::enums::inputPort::port1:
//		return mUi->port1Box;
//	case robots::enums::inputPort::port2:
//		return mUi->port2Box;
//	case robots::enums::inputPort::port3:
//		return mUi->port3Box;
//	case robots::enums::inputPort::port4:
//		return mUi->port4Box;
//	case robots::enums::inputPort::none:
//		break;
//	}
	return nullptr;
}

void D2ModelWidget::addPort(int const index)
{
	if (!isVisible() && mFirstShow) {
		return;
	}

	QComboBox * const comboBox = dynamic_cast<QComboBox *>(sender());
	PortInfo const port = mComboBoxesToPortsMap.value(comboBox);
	DeviceInfo const device = comboBox->itemData(index).value<DeviceInfo>();

	resetButtons();

	deviceConfigurationChanged(mRobotModel.name(), port, device);
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
			if (mCurrentWall->realShape().intersects(mRobot->realBoundingRect())) {
				mCurrentWall->setX2andY2(oldPos.x(), oldPos.y());
			}
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

	mRobot->checkSelection();
	foreach (SensorItem *sensor, mSensors) {
		if (sensor) {
			sensor->checkSelection();
		}
	}

	QPointF const position = mouseEvent->scenePos();
	processDragMode();

	switch (mDrawingAction) {
	case enums::drawingAction::wall: {
		if (!mRobot->realBoundingRect().intersects(QRectF(position, position))) {
			mCurrentWall = new WallItem(position, position);
			mScene->removeMoveFlag(mouseEvent, mCurrentWall);
			mWorldModel->addWall(mCurrentWall);
			mMouseClicksCount++;
		}
		break;
	}
	case enums::drawingAction::line: {
		mCurrentLine = new LineItem(position, position);
		mCurrentLine->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
				, mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentLine);
		mWorldModel->addColorField(mCurrentLine);
		mMouseClicksCount++;
		break;
	}
	case enums::drawingAction::stylus: {
		mCurrentStylus = new StylusItem(position.x(), position.y());
		mCurrentStylus->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
				, mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentStylus);
		mWorldModel->addColorField(mCurrentStylus);
		mMouseClicksCount++;
		break;
	}
	case enums::drawingAction::ellipse: {
		mCurrentEllipse = new EllipseItem(position, position);
		mCurrentEllipse->setPen(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentEllipse);
		mWorldModel->addColorField(mCurrentEllipse);
		mMouseClicksCount++;
		break;
	}
	case enums::drawingAction::none: {
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
		mRobot->checkSelection();
		foreach (SensorItem *sensor, mSensors) {
			if (sensor) {
				sensor->checkSelection();
			}
		}
	}

	bool needUpdate = true;
	processDragMode();
	switch (mDrawingAction){
	case enums::drawingAction::wall:
		reshapeWall(mouseEvent);
		break;
	case enums::drawingAction::line:
		reshapeLine(mouseEvent);
		break;
	case enums::drawingAction::stylus:
		reshapeStylus(mouseEvent);
		break;
	case enums::drawingAction::ellipse:
		reshapeEllipse(mouseEvent);
		break;
	default:
		needUpdate = false;
		if (mouseEvent->buttons() & Qt::LeftButton) {
			mScene->forMoveResize(mouseEvent, mRobot->realBoundingRect());
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

	mRobot->checkSelection();
	foreach (SensorItem *sensor, mSensors) {
		if (sensor) {
			sensor->checkSelection();
		}
	}

	processDragMode();

	switch (mDrawingAction){
	case enums::drawingAction::wall: {
		reshapeWall(mouseEvent);
		mCurrentWall->setSelected(true);
		mCurrentWall = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case enums::drawingAction::line: {
		reshapeLine(mouseEvent);
		mCurrentLine->setSelected(true);
		mCurrentLine = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case enums::drawingAction::stylus: {
		reshapeStylus(mouseEvent);
		mCurrentStylus->setSelected(true);
		mCurrentStylus = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	case enums::drawingAction::ellipse: {
		reshapeEllipse(mouseEvent);
		mCurrentEllipse->setSelected(true);
		mCurrentEllipse = nullptr;
		mMouseClicksCount = 0;
		break;
	}
	default:
		mScene->forReleaseResize(mouseEvent, mRobot->realBoundingRect());
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
	foreach (WallItem const *wall, mWorldModel->walls()) {
		if (wall->realShape().intersects(mRobot->realBoundingRect())) {
			mRobot->recoverDragStartPosition();
			return;
		}
	}
}

void D2ModelWidget::removeSensor(PortInfo const &port)
{
	// Here's the point where all interested entities are notified about sensor deletion,
	// so if this code gets broken or worked around, we'll have some almost undebuggable
	// dangling pointers in scene and in robot item. But what could possibly go wrong?
	if (!mSensors.contains(port) || !mSensors.value(port) || !mRobot) {
		return;
	}

	mRobot->removeSensor(mSensors[port]);
	mScene->removeItem(mSensors[port]);
	delete mSensors[port];
	mSensors[port] = nullptr;
}

void D2ModelWidget::reinitSensor(PortInfo const &port)
{
	removeSensor(port);

	DeviceInfo const &device = currentConfiguration(mRobotModel.name(), port);

	if (device.isNull()) {
		return;
	}

	SensorItem *sensor = device.isA<RangeSensor>()
			? new SonarSensorItem(*mWorldModel, mTwoDRobotModel->configuration(), port)
			: new SensorItem(mTwoDRobotModel->configuration(), port);

	mRobot->addSensor(sensor);
	mScene->addItem(sensor);

	sensor->addStickyItem(mRobot);

	Rotater * const rotater = new Rotater();
	rotater->setMasterItem(sensor);
	rotater->setVisible(false);
	sensor->setRotater(rotater);
	sensor->setRotation(mTwoDRobotModel->configuration().direction(port));

	sensor->setParentItem(mRobot);
	sensor->setPos(mRobot->mapFromScene(mTwoDRobotModel->configuration().position(port)));

	mSensors[port] = sensor;
}

void D2ModelWidget::deleteItem(QGraphicsItem *item)
{
	/// @todo Handle all cases equally
	SensorItem * const sensor = dynamic_cast<SensorItem *>(item);
	if (sensor) {
		PortInfo const port = mSensors.key(sensor, PortInfo());
		if (port.isValid()) {
			removeSensor(port);
		}
		return;
	}

	WallItem * const wall = dynamic_cast<WallItem *>(item);
	if (wall) {
		mScene->removeItem(wall);
		mWorldModel->removeWall(wall);
	}

	ColorFieldItem *colorField = dynamic_cast<ColorFieldItem *>(item);
	if (colorField) {
		mScene->removeItem(colorField);
		mWorldModel->removeColorField(colorField);
		delete colorField;
	}
}

bool D2ModelWidget::isColorItem(AbstractItem *item)
{
	RobotItem* robotItem = dynamic_cast<RobotItem*>(item);
	SensorItem* sensorItem = dynamic_cast<SensorItem*>(item);
	WallItem* wallItem = dynamic_cast<WallItem*>(item);
	Rotater* rotaterItem = dynamic_cast<Rotater*>(item);
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

	if(mDrawingAction == enums::drawingAction::none) {
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

D2ModelScene* D2ModelWidget::scene()
{
	return mScene;
}

//void D2ModelWidget::setSensorVisible(robots::enums::inputPort::InputPortEnum port, bool isVisible)
//{
//	if (mSensors[port]) {
//		mSensors[port]->setVisible(isVisible);
//	}
//}

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
	emit d2WasClosed();
}

//QVector<SensorItem *> D2ModelWidget::sensorItems() const
//{
//	return mSensors;
//}

void D2ModelWidget::saveToRepo()
{
	emit modelChanged(generateXml());
}

QDomDocument D2ModelWidget::generateXml() const
{
	QDomDocument save;
	QDomElement root = save.createElement("root");
	save.appendChild(root);
	root.appendChild(mWorldModel->serialize(save, QPoint(0, 0)));
	mTwoDRobotModel->serialize(save);
	return save;
}

void D2ModelWidget::loadXml(QDomDocument const &worldModel)
{
	clearScene(true);
	QDomNodeList const worldList = worldModel.elementsByTagName("world");
	QDomNodeList const robotList = worldModel.elementsByTagName("robot");
	if (worldList.count() != 1 || robotList.count() != 1) {
		/// @todo Report error
		return;
	}

	mWorldModel->deserialize(worldList.at(0).toElement());
	mTwoDRobotModel->deserialize(robotList.at(0).toElement());

	for (int i = 0; i < 4; ++i) {
//		reinitSensor(static_cast<robots::enums::inputPort::InputPortEnum>(i));
	}

	mRobot->processPositionAndAngleChange();
	mDrawingAction = enums::drawingAction::noneWordLoad;
	update();
	mDrawingAction = enums::drawingAction::none;
}

void D2ModelWidget::worldWallDragged(WallItem *wall, const QPainterPath &shape
		, QPointF const &oldPos)
{
	bool const isNeedStop = shape.intersects(mRobot->realBoundingRect());
	wall->onOverlappedWithRobot(isNeedStop);
	if (wall->isDragged() && ((mDrawingAction == enums::drawingAction::none) ||
			(mDrawingAction == enums::drawingAction::wall && mCurrentWall == wall)))
	{
		wall->setFlag(QGraphicsItem::ItemIsMovable, !isNeedStop);
		if (isNeedStop) {
			wall->setPos(oldPos);
		}
	}
}

void D2ModelWidget::enableRobotFollowing(bool on)
{
	mFollowRobot = on;
	qReal::SettingsManager::setValue("2dFollowingRobot", on);
}

void D2ModelWidget::setCursorType(enums::cursorType::CursorType cursor)
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

	static_cast<D2RobotModel *>(mTwoDRobotModel)->setNoiseSettings();
}

void D2ModelWidget::startTimelineListening()
{
	mUi->timelineBox->setValue(0);
	connect(mTimeline, SIGNAL(tick()), this, SLOT(onTimelineTick()), Qt::UniqueConnection);
}

void D2ModelWidget::stopTimelineListening()
{
	disconnect(mTimeline, SIGNAL(tick()), this, SLOT(onTimelineTick()));
}

void D2ModelWidget::onTimelineTick()
{
	mUi->timelineBox->stepBy(1);
}

void D2ModelWidget::toggleDisplayVisibility()
{
//	setDisplayVisibility(!mNxtDisplay->isVisible());
}

void D2ModelWidget::setDisplayVisibility(bool visible)
{
//	mNxtDisplay->setVisible(visible);
	QString const direction = visible ? "right" : "left";
	mUi->displayButton->setIcon(QIcon(QString(":/icons/2d_%1.png").arg(direction)));
	SettingsManager::setValue("2d_displayVisible", visible);
}

QGraphicsView::DragMode D2ModelWidget::cursorTypeToDragType(enums::cursorType::CursorType type) const
{
	switch(type) {
	case enums::cursorType::NoDrag:
	case enums::cursorType::drawEllipse:
	case enums::cursorType::drawLine:
	case enums::cursorType::drawStylus:
	case enums::cursorType::drawWall:
		return QGraphicsView::NoDrag;
	case enums::cursorType::hand:
		return QGraphicsView::ScrollHandDrag;
	case enums::cursorType::multiselection:
		return QGraphicsView::RubberBandDrag;
	default:
		return QGraphicsView::ScrollHandDrag;
	}
}

QCursor D2ModelWidget::cursorTypeToCursor(enums::cursorType::CursorType type) const
{
	switch(type) {
	case enums::cursorType::NoDrag:
		return QCursor(Qt::ArrowCursor);
	case enums::cursorType::hand:
		return QCursor(Qt::OpenHandCursor);
	case enums::cursorType::multiselection:
		return QCursor(Qt::ArrowCursor);
	case enums::cursorType::drawLine:
		return QCursor(QPixmap(":/icons/2d_drawLineCursor.png"), 0, 0);
	case enums::cursorType::drawWall:
		return QCursor(QPixmap(":/icons/2d_drawWallCursor.png"), 0, 0);
	case enums::cursorType::drawEllipse:
		return QCursor(QPixmap(":/icons/2d_drawEllipseCursor.png"), 0, 0);
	case enums::cursorType::drawStylus:
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
		setCursorType(enums::cursorType::hand);
	}
}

void D2ModelWidget::onMultiselectionCursorButtonToggled(bool on)
{
	if (on) {
		setHighlightOneButton(mUi->multiselectionCursorButton);
		setCursorType(enums::cursorType::multiselection);
	}
}

void D2ModelWidget::syncCursorButtons()
{
	switch(mNoneCursorType) {
	case enums::cursorType::hand:
		mUi->handCursorButton->setChecked(true);
		break;
	case enums::cursorType::multiselection:
		mUi->multiselectionCursorButton->setChecked(true);
		break;
	default:
		break;
	}
}

void D2ModelWidget::alignWalls()
{
	for (WallItem * const wall : mWorldModel->walls()) {
		if (mScene->items().contains(wall)) {
			wall->setBeginCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
			wall->setEndCoordinatesWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		}
	}
}

void D2ModelWidget::onDeviceConfigurationChanged(const QString &robotModel
		, const PortInfo &port, const DeviceInfo &device)
{
	if (robotModel != mRobotModel.name()) {
		/// @todo Convert configuration between models or something?
		return;
	}

	updateWheelComboBoxes();

	/// @todo Isomorphic map with constant lookup both ways?
	QComboBox *comboBox = mComboBoxesToPortsMap.key(port, nullptr);
	if (!comboBox) {
		/// @todo Report internal error.
		return;
	}

	for (int index = 0; index < comboBox->count(); ++index) {
		if (comboBox->itemData(index).value<DeviceInfo>() == device) {
			comboBox->setCurrentIndex(index);
			break;
		}
	}

	QPointF const sensorPos = mSensors.contains(port) && mSensors.value(port)
			? mSensors.value(port)->scenePos()
			: mRobot->mapToScene(mRobot->boundingRect().center() + QPoint(mRobot->boundingRect().width(), 0));

	mTwoDRobotModel->configuration().setSensor(port, device, sensorPos.toPoint(), 0);
	reinitSensor(port);
}

void D2ModelWidget::initRunStopButtons()
{
	connect(mUi->runButton, &QPushButton::clicked, this, &D2ModelWidget::runButtonPressed);
	connect(mUi->stopButton, &QPushButton::clicked, this, &D2ModelWidget::stopButtonPressed);
}

void D2ModelWidget::updateWheelComboBoxes()
{
	/// @todo Automatically configure wheel ports if there is only two motors are used in a program.
	PortInfo const leftWheelOldPort = mUi->leftWheelComboBox->currentData().value<PortInfo>();
	PortInfo const rightWheelOldPort = mUi->rightWheelComboBox->currentData().value<PortInfo>();

	mUi->leftWheelComboBox->clear();
	mUi->rightWheelComboBox->clear();

	mUi->leftWheelComboBox->addItem("None", QVariant::fromValue(PortInfo("None")));
	mUi->rightWheelComboBox->addItem("None", QVariant::fromValue(PortInfo("None")));

	for (PortInfo const &port : mRobotModel.availablePorts()) {
		for (DeviceInfo const &device : mRobotModel.allowedDevices(port)) {
			if (device.isA<Motor>()) {
				QString const item = device.friendlyName() + " (port " + port.name() + ")";
				mUi->leftWheelComboBox->addItem(item, QVariant::fromValue(port));
				mUi->rightWheelComboBox->addItem(item, QVariant::fromValue(port));
			}
		}
	}

	auto restoreOldPort = [](QComboBox * const comboBox, PortInfo const &oldPort) {
		for (int i = 0; i < comboBox->count(); ++i) {
			if (comboBox->itemData(i).value<PortInfo>() == oldPort) {
				comboBox->setCurrentIndex(i);
				return true;
			}
		}

		return false;
	};

	if (!restoreOldPort(mUi->leftWheelComboBox, leftWheelOldPort)) {
		if (mUi->leftWheelComboBox->count() > 1) {
			mUi->leftWheelComboBox->setCurrentIndex(1);
		}
	}

	if (!restoreOldPort(mUi->rightWheelComboBox, rightWheelOldPort)) {
		if (mUi->rightWheelComboBox->count() > 2) {
			mUi->rightWheelComboBox->setCurrentIndex(2);
		} else if (mUi->rightWheelComboBox->count() > 1) {
			mUi->rightWheelComboBox->setCurrentIndex(1);
		}
	}
}
