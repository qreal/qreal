#include "d2ModelWidget.h"
#include "ui_d2Form.h"

#include <QtGui/QFileDialog>
#include <QtGui/QRegion>
#include <QtCore/qmath.h>

#include "sensorItem.h"
#include "sonarSensorItem.h"
#include "rotater.h"
#include "../../../../../qrutils/outFile.h"
#include "../../../../../qrutils/xmlUtils.h"
#include "../../../../../qrkernel/settingsManager.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

D2ModelWidget::D2ModelWidget(RobotModelInterface *robotModel, WorldModel *worldModel, QWidget *parent)
		: QWidget(parent)
		, mUi(new Ui::D2Form)
		, mScene(NULL)
		, mRobot(NULL)
		, mDrawCyclesCount(0)
		, mMaxDrawCyclesBetweenPathElements(SettingsManager::value("drawCyclesBetweenPathElements", 500).toInt())
		, mRobotModel(robotModel)
		, mWorldModel(worldModel)
		, mDrawingAction(drawingAction::none)
		, mMouseClicksCount(0)
		, mCurrentWall(NULL)
		, mCurrentLine(NULL)
		, mCurrentStylus(NULL)
		, mCurrentPort(inputPort::none)
		, mCurrentSensorType(sensorType::unused)
		, mButtonsCount(8) // magic numbers are baaad, mkay?
{
	setWindowIcon(QIcon(":/icons/kcron.png"));

	initWidget();

	connectUiButtons();

	connect(mScene, SIGNAL(mouseClicked(QGraphicsSceneMouseEvent *)), this, SLOT(mouseClicked(QGraphicsSceneMouseEvent *)));
	connect(mScene, SIGNAL(mouseMoved(QGraphicsSceneMouseEvent*)), this, SLOT(mouseMoved(QGraphicsSceneMouseEvent*)));
	connect(mScene, SIGNAL(mouseReleased(QGraphicsSceneMouseEvent*)), this, SLOT(mouseReleased(QGraphicsSceneMouseEvent*)));
	connect(mScene, SIGNAL(itemDeleted(QGraphicsItem*)), this, SLOT(deleteItem(QGraphicsItem*)));

	connect(mScene, SIGNAL(selectionChanged()), this, SLOT(changePalette()));
}

D2ModelWidget::~D2ModelWidget()
{
	delete mRobot;
	delete mScene;
	delete mUi;
}

void D2ModelWidget::initWidget()
{
	mUi->setupUi(this);
	mSensors.resize(4);
	mSensorRotaters.resize(4);

	mScene = new D2ModelScene(mUi->graphicsView);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	move(0, 0);

	mUi->penWidthSpinBox->setRange(6, 13);

	QStringList colorNames;
	colorNames.push_back("Black");
	colorNames.push_back("Blue");
	colorNames.push_back("Green");
	colorNames.push_back("Yellow");
	colorNames.push_back("Red");

	mUi->penColorComboBox->setColorList(colorNames);
	mUi->penColorComboBox->setColor(QColor("black"));
}

void D2ModelWidget::connectUiButtons()
{
	connect(mUi->stylusButton, SIGNAL(toggled(bool)), this, SLOT(addStylus(bool)));
	connect(mUi->lineButton, SIGNAL(toggled(bool)), this, SLOT(addLine(bool)));
	connect(mUi->wallButton, SIGNAL(toggled(bool)), this, SLOT(addWall(bool)));
	connect(mUi->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));

	connect(mUi->penWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changePenWidth(int)));
	connect(mUi->penColorComboBox, SIGNAL(activated(const QString &)), this, SLOT(changePenColor(const QString &)));

	connect(mUi->saveWorldModelPushButton, SIGNAL(clicked()), this, SLOT(saveWorldModel()));
	connect(mUi->loadWorldModelPushButton, SIGNAL(clicked()), this, SLOT(loadWorldModel()));

	connect(&mPortsMapper, SIGNAL(mapped(int)), this, SLOT(addPort(int)));

	connect(mUi->port1Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port1Box, inputPort::port1);
	connect(mUi->port2Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port2Box, inputPort::port2);
	connect(mUi->port3Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port3Box, inputPort::port3);
	connect(mUi->port4Box, SIGNAL(activated(int)), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port4Box, inputPort::port4);

	connect(mUi->speedComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSpeed(int)));
}

void D2ModelWidget::changeSpeed(int curIndex)
{
	switch(curIndex){
	case 0:
		mRobotModel->speed(1);
		break;
	case 1:
		mRobotModel->speed(2);
		break;
	case 2:
		mRobotModel->speed(4);
		break;
	default:
		mRobotModel->speed(1);
	}
}



void D2ModelWidget::init(bool isActive)
{
	if (!isActive){
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
	update();
}

void D2ModelWidget::setD2ModelWidgetActions(QAction *runAction, QAction *stopAction)
{
	connect(mUi->runButton, SIGNAL(clicked()), runAction, SIGNAL(triggered()));
	connect(mUi->stopButton, SIGNAL(clicked()), stopAction, SIGNAL(triggered()));
}

void D2ModelWidget::drawInitialRobot()
{
	mRobot = new RobotItem();
	connect(mRobot, SIGNAL(changedPosition()), this, SLOT(handleNewRobotPosition()));
	mScene->addItem(mRobot);

	mRotater = new Rotater();
	mRotater->setMasterItem(mRobot);
	mRotater->setVisible(true);
	mScene->addItem(mRotater);

	mRotater->setVisible(false);

	mRobot->setRotater(mRotater);
	mRobot->setRobotModel(mRobotModel);

	mUi->graphicsView->centerOn(mRobot);
}

QPointF D2ModelWidget::robotPos() const
{
	return mRobot ? mRobot->pos() : QPointF(0,0);
}

void D2ModelWidget::close()
{
	if (mRobot != NULL) {
		mRobot->resetTransform();
		mRobotPath.clear();
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

bool D2ModelWidget::isRobotOnTheGround()
{
	return mRobot ? mRobot->isOnTheGround() : false;
}

void D2ModelWidget::draw(QPointF newCoord, qreal angle, QPointF dPoint)
{
	mAngleOld = angle;
	mRotatePointOld = dPoint;
	mRobot->setPos(newCoord);
	mRobot->setTransform(QTransform().translate(dPoint.x(), dPoint.y()).rotate(angle).translate(-dPoint.x(), -dPoint.y()));

	++mDrawCyclesCount;

	if (mDrawCyclesCount > mMaxDrawCyclesBetweenPathElements) {
		// Here we place a green circle in a center of a robot

		QPointF const robotCenterPos = mRobot->sceneBoundingRect().center();

		// This is a rectangle where the circle will be located, first placing its top-left corner in robot center, then moving it
		// so that its center becomes robot center
		QRectF const pathElementRect = QRectF(robotCenterPos, robotCenterPos + QPointF(4, 4)).translated(QPointF(-2, -2));
		QGraphicsItem * const pathElement = mScene->addEllipse(pathElementRect, QPen(Qt::green), QBrush(Qt::green));

		// Adding resulting element to a path
		mRobotPath << pathElement;
		mDrawCyclesCount = 0;
	}

	QRectF const viewPortRect = mUi->graphicsView->mapToScene(mUi->graphicsView->viewport()->rect()).boundingRect();
	if (!viewPortRect.contains(mRobot->sceneBoundingRect().toRect())) {
		QRectF const requiredViewPort = viewPortRect.translated(mRobot->scenePos() - viewPortRect.center());
		mScene->setSceneRect(mScene->itemsBoundingRect().united(requiredViewPort));
		mUi->graphicsView->centerOn(mRobot);
	}
}

void D2ModelWidget::drawWalls()
{
	foreach (WallItem *wall, mWorldModel->walls()) {
		mScene->addItem(wall);
	}
}

void D2ModelWidget::drawColorFields()
{
	foreach (ColorFieldItem *colorField, mWorldModel->colorFields()) {
		mScene->addItem(colorField);
	}
}

void D2ModelWidget::drawBeep(QColor const &color)
{
	Q_UNUSED(color)
}

QPolygonF const D2ModelWidget::robotBoundingPolygon(QPointF const &coord, qreal const &angle) const
{
	Q_UNUSED(coord);
	Q_UNUSED(angle);

	return mRobot->mapToScene(QPolygon(mRobot->boundingRect().toRect()));
}

void D2ModelWidget::addWall(bool on)
{
	if (!on) {
		mDrawingAction = drawingAction::none;
		mMouseClicksCount = 0;
		return;
	}

	mDrawingAction = drawingAction::wall;
}

void D2ModelWidget::addLine(bool on)
{
	if (!on) {
		mDrawingAction = drawingAction::none;
		mMouseClicksCount = 0;
		return;
	}

	mDrawingAction = drawingAction::line;
}

void D2ModelWidget::addStylus(bool on)
{
	if (!on) {
		mDrawingAction = drawingAction::none;
		mMouseClicksCount = 0;
		return;
	}

	mDrawingAction = drawingAction::stylus;
}

void D2ModelWidget::clearScene()
{
	mWorldModel->clearScene();
	removeSensor(inputPort::port1);
	removeSensor(inputPort::port2);
	removeSensor(inputPort::port3);
	removeSensor(inputPort::port4);
	int const noneSensorIndex = 0;
	mUi->port1Box->setCurrentIndex(noneSensorIndex);
	mUi->port2Box->setCurrentIndex(noneSensorIndex);
	mUi->port3Box->setCurrentIndex(noneSensorIndex);
	mUi->port4Box->setCurrentIndex(noneSensorIndex);
	mRobotModel->clear();
	mScene->clear();
	mRobotPath.clear();
	drawInitialRobot();
}

void D2ModelWidget::resetButtons()
{
	mCurrentWall = NULL;
	mCurrentLine = NULL;
	mCurrentStylus = NULL;
	mMouseClicksCount = 0;
	mDrawingAction = drawingAction::none;
}

QComboBox *D2ModelWidget::currentComboBox()
{
	switch (mCurrentPort){
	case inputPort::port1:
		return mUi->port1Box;
	case inputPort::port2:
		return mUi->port2Box;
	case inputPort::port3:
		return mUi->port3Box;
	case inputPort::port4:
		return mUi->port4Box;
	case inputPort::none:
		break;
	}
	return NULL;
}

void D2ModelWidget::addPort(int const port)
{
	mCurrentPort = static_cast<inputPort::InputPortEnum>(port);

	switch (currentComboBox()->currentIndex()){
	case 0:
		mCurrentSensorType = sensorType::unused;
		break;
	case 1:
		mCurrentSensorType = sensorType::touchBoolean;
		break;
	case 2:
		mCurrentSensorType = sensorType::colorFull;
		break;
	case 3:
		mCurrentSensorType = sensorType::sonar;
		break;

	}
	QPointF newpos = mRobot->mapFromScene(mRobot->boundingRect().center());
	mRobotModel->configuration().setSensor(mCurrentPort, mCurrentSensorType, newpos.toPoint(), 0);
	reinitSensor(mCurrentPort);

	resetButtons();

}

void D2ModelWidget::reshapeWall(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentWall != NULL) {
		mCurrentWall->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier)
			mCurrentWall->reshapeRectWithShift();
	}
}

void D2ModelWidget::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentLine != NULL) {
		mCurrentLine->setX2andY2(pos.x(), pos.y());
		if (event->modifiers() & Qt::ShiftModifier)
			mCurrentLine->reshapeRectWithShift();
	}
}

void D2ModelWidget::reshapeStylus(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentStylus != NULL) {
		mCurrentStylus->addLine(pos.x(), pos.y());
	}
}

void D2ModelWidget::mouseClicked(QGraphicsSceneMouseEvent *mouseEvent)
{
	mRobot->checkSelection();
	foreach (SensorItem *sensor, mSensors) {
		if (sensor != NULL) {
			sensor->checkSelection();
		}
	}

	QPointF const position = mouseEvent->scenePos();
	mScene->setDragMode(mDrawingAction);
	switch (mDrawingAction){
	case drawingAction::wall: {
		mCurrentWall = new WallItem(position, position);
		mScene->removeMoveFlag(mouseEvent, mCurrentWall);
		mWorldModel->addWall(mCurrentWall);
		mMouseClicksCount++;
	}
		break;
	case drawingAction::line: {
		mCurrentLine = new LineItem(position, position);
		mCurrentLine->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
								  , mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentLine);
		mWorldModel->addColorField(mCurrentLine);
		mMouseClicksCount++;
	}
		break;
	case drawingAction::stylus: {
		mCurrentStylus = new StylusItem(position.x(), position.y());
		mCurrentStylus->setPenBrush(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems()
									, mScene->brushStyleItems(), mScene->brushColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentStylus);
		mWorldModel->addColorField(mCurrentStylus);
		mMouseClicksCount++;
	}
		break;

	case drawingAction::none: {
		mMouseClicksCount = 0;
		mScene->forPressResize(mouseEvent);
	}
		break;
	default:
		break;
	}

	update();
}

void D2ModelWidget::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
	mRobot->checkSelection();
	foreach (SensorItem *sensor, mSensors) {
		if (sensor != NULL) {
			sensor->checkSelection();
		}
	}

	mScene->setDragMode(mDrawingAction);
	switch (mDrawingAction){
	case drawingAction::wall:
		reshapeWall(mouseEvent);
		break;
	case drawingAction::line:
		reshapeLine(mouseEvent);
		break;
	case drawingAction::stylus:
		reshapeStylus(mouseEvent);
		break;
	default:
		mScene->forMoveResize(mouseEvent);
		break;
	}
	mScene->update();
}

void D2ModelWidget::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
	mRobot->checkSelection();
	foreach (SensorItem *sensor, mSensors) {
		if (sensor != NULL) {
			sensor->checkSelection();
		}
	}

	mScene->setDragMode(mDrawingAction);

	switch (mDrawingAction){
	case drawingAction::wall: {
		reshapeWall(mouseEvent);
		mCurrentWall = NULL;
		mMouseClicksCount = 0;
		mDrawingAction = drawingAction::none;
	}
		break;
	case drawingAction::line: {
		reshapeLine(mouseEvent);
		mCurrentLine = NULL;
		mMouseClicksCount = 0;
		mDrawingAction = drawingAction::none;
	}
		break;
	case drawingAction::stylus: {
		reshapeStylus(mouseEvent);
		mCurrentStylus = NULL;
		mMouseClicksCount = 0;
		mDrawingAction = drawingAction::none;
	}
		break;
	default:
		mScene->forReleaseResize(mouseEvent);
		break;
	}
	mUi->wallButton->setChecked(false);
	mUi->lineButton->setChecked(false);
	mUi->stylusButton->setChecked(false);
	mScene->setMoveFlag(mouseEvent);
	mScene->update();
}

void D2ModelWidget::saveWorldModel()
{
	// Saves world and robot models simultaneously, for now.
	QString const saveFileName = QFileDialog::getSaveFileName(this, tr("Saving world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (saveFileName.isEmpty())
		return;

	QDomDocument save;
	QDomElement root = save.createElement("root");
	save.appendChild(root);
	root.appendChild(mWorldModel->serialize(save, QPoint(0, 0)));
	root.appendChild(mRobotModel->configuration().serialize(save));

	utils::OutFile saveFile(saveFileName);
	saveFile() << "<?xml version='1.0' encoding='utf-8'?>\n";
	saveFile() << save.toString(4);
}

void D2ModelWidget::loadWorldModel()
{
	// Loads world and robot models simultaneously.
	QString const saveFileName = QFileDialog::getOpenFileName(this, tr("Saving world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (saveFileName.isEmpty()) {
		return;
	}

	QDomDocument const save = utils::xmlUtils::loadDocument(saveFileName);

	QDomNodeList const worldList = save.elementsByTagName("world");
	QDomNodeList const robotList = save.elementsByTagName("robot");
	if (worldList.count() != 1 || robotList.count() != 1) {
		// TODO: Report error
		return;
	}

	mWorldModel->deserialize(worldList.at(0).toElement());
	mRobotModel->configuration().deserialize(robotList.at(0).toElement());

	for (int i = 0; i < 4; ++i) {
		reinitSensor(static_cast<inputPort::InputPortEnum>(i));
	}

	update();
}

void D2ModelWidget::handleNewRobotPosition()
{
	foreach (QGraphicsItem * const item, mRobotPath) {
		mScene->removeItem(item);
	}
}

void D2ModelWidget::removeSensor(inputPort::InputPortEnum port)
{
	// Here's the point where all interested entities are notified about sensor deletion,
	// so if this code gets broken or worked around, we'll have some almost undebuggable
	// dangling pointers in scene and in robot item. But what could possibly go wrong?
	if (mSensors[port]) {
		mRobot->removeSensor(mSensors[port]);
		mScene->removeItem(mSensors[port]);
		mScene->removeItem(mSensorRotaters[port]);
		delete mSensors[port];
		delete mSensorRotaters[port];
		mSensors[port] = NULL;
		mSensorRotaters[port] = NULL;
	}
}

void D2ModelWidget::reinitSensor(inputPort::InputPortEnum port)
{
	removeSensor(port);

	if (mRobotModel->configuration().type(port) == sensorType::unused) {
		return;
	}

	SensorItem *sensor = mRobotModel->configuration().type(port) == sensorType::sonar
			? new SonarSensorItem(*mWorldModel, mRobotModel->configuration(), port)
			: new SensorItem(mRobotModel->configuration(), port);
	sensor->setRotatePoint(rotatePoint);

	mRobot->addSensor(sensor);
	mScene->addItem(sensor);

	// Setting sensor rotaters
	mSensorRotaters[port] = new Rotater();
	mSensorRotaters[port]->setMasterItem(sensor);
	mSensorRotaters[port]->setVisible(false);
	sensor->setRotater(mSensorRotaters[port]);

	sensor->setBasePosition(mRobot->basePoint(), mRobot->rotateAngle()/*, rotatePoint*/);
	mScene->addItem(mSensorRotaters[port]);

	mSensors[port] = sensor;
}

void D2ModelWidget::deleteItem(QGraphicsItem *item)
{
	SensorItem * const sensor = dynamic_cast<SensorItem *>(item);
	if (!sensor) {
		return;
	}

	int const port = mSensors.indexOf(sensor);
	if (port != -1) {
		removeSensor(static_cast<inputPort::InputPortEnum>(port));
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
	foreach (QGraphicsItem *graphicsItem, listSelectedItems) {
		AbstractItem* item = dynamic_cast<AbstractItem*>(graphicsItem);
		if (item != NULL) {
			//теперь надо исключить еще те объекты, которым не надо менять цвет и т.д., а т.е. робота, сенсоры, стены и ротатеры
			if (isColorItem(item))
				resList.push_back(item);
		}
	}
	qSort(resList.begin(), resList.end(), mScene->compareItems);
	return resList;
}

void D2ModelWidget::changePenWidth(int width)
{
	mScene->setPenWidthItems(width);
	foreach (AbstractItem *item, selectedColorItems()) {
		item->setPenWidth(width);
	}
	mScene->update();
}

void D2ModelWidget::changePenColor(const QString &text)
{
	mScene->setPenColorItems(text);
	foreach (AbstractItem *item, selectedColorItems()) {
		item->setPenColor(text);
	}
	mScene->update();
}

void D2ModelWidget::changePalette()
{
	if(mDrawingAction == drawingAction::none) {
		QList<QGraphicsItem *> listSelectedItems = mScene->selectedItems();
		if (listSelectedItems.isEmpty()) {
			setNoPalette();
			mScene->setEmptyPenBrushItems();
		}
		else {
			AbstractItem* item = dynamic_cast<AbstractItem*>(listSelectedItems.back());
			if (isColorItem(item)) {
				QPen penItem = item->pen();
				QBrush brushItem = item->brush();
				setItemPalette(penItem, brushItem);
				mScene->setPenBrushItems(penItem, brushItem);
			}
		}
	}
}

void D2ModelWidget::setValuePenColorComboBox(QColor penColor)
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
	mUi->penWidthSpinBox->setValue(6);
	mUi->penColorComboBox->setColor(QColor("black"));
}

D2ModelScene* D2ModelWidget::scene()
{
	return mScene;
}

void D2ModelWidget::setSensorVisible(inputPort::InputPortEnum port, bool isVisible)
{
	if (mSensors[port]) {
		mSensors[port]->setVisible(isVisible);
	}
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
	emit d2WasClosed();
}
