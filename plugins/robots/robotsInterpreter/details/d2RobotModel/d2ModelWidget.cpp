#include "d2ModelWidget.h"
#include "ui_d2Form.h"

#include <QtGui/QFileDialog>

#include "sensorItem.h"
#include "sonarSensorItem.h"
#include "rotater.h"
#include "../../../../../qrutils/outFile.h"
#include "../../../../../qrutils/xmlUtils.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

D2ModelWidget::D2ModelWidget(RobotModelInterface *robotModel, WorldModel *worldModel, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::D2Form)
	, mScene(NULL)
	, mRobot(NULL)
	, mPolygon(NULL)
	, mRobotModel(robotModel)
	, mWorldModel(worldModel)
	, mDrawingAction(drawingAction::none)
	, mMouseClicksCount(0)
	, mCurrentPort(inputPort::none)
	, mCurrentSensorType(sensorType::unused)
	, mButtonsCount(6) // magic numbers are baaad, mkay?
{
	mUi->setupUi(this);
	mSensors.resize(4);

	mScene = new D2ModelScene(mUi->graphicsView);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	move(0, 0);

	connectUiButtons();

	connect(mScene, SIGNAL(mouseClicked(QGraphicsSceneMouseEvent *)), this, SLOT(mouseClicked(QGraphicsSceneMouseEvent *)));
	connect(mScene, SIGNAL(itemDeleted(QGraphicsItem*)), this, SLOT(deleteItem(QGraphicsItem*)));
}

D2ModelWidget::~D2ModelWidget()
{
	delete mPolygon;
	delete mRobot;
	delete mScene;
	delete mUi;
}

void D2ModelWidget::connectUiButtons()
{
	connect(mUi->wallButton, SIGNAL(toggled(bool)), this, SLOT(addWall(bool)));
	connect(mUi->clearButton, SIGNAL(clicked()), this, SLOT(clearScene()));
	connect(mUi->doNothingButton, SIGNAL(clicked()), this, SLOT(resetButtons()));

	connect(mUi->port1AddButton, SIGNAL(clicked()), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port1AddButton, inputPort::port1);
	connect(mUi->port2AddButton, SIGNAL(clicked()), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port2AddButton, inputPort::port2);
	connect(mUi->port3AddButton, SIGNAL(clicked()), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port3AddButton, inputPort::port3);
	connect(mUi->port4AddButton, SIGNAL(clicked()), &mPortsMapper, SLOT(map()));
	mPortsMapper.setMapping(mUi->port4AddButton, inputPort::port4);

	connect(mUi->saveWorldModelPushButton, SIGNAL(clicked()), this, SLOT(saveWorldModel()));
	connect(mUi->loadWorldModelPushButton, SIGNAL(clicked()), this, SLOT(loadWorldModel()));

	connect(&mPortsMapper, SIGNAL(mapped(int)), this, SLOT(addPort(int)));

	connect(mUi->port1Box, SIGNAL(activated(int)), mUi->port1AddButton, SLOT(click()));
	connect(mUi->port2Box, SIGNAL(activated(int)), mUi->port2AddButton, SLOT(click()));
	connect(mUi->port3Box, SIGNAL(activated(int)), mUi->port3AddButton, SLOT(click()));
	connect(mUi->port4Box, SIGNAL(activated(int)), mUi->port4AddButton, SLOT(click()));
}

void D2ModelWidget::init(bool isActive)
{
	if (!isActive){
		hide();
		return;
	}

	if (!mRobot)
		drawInitialRobot();

	mUi->graphicsView->show();
	show();
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
	mRobot->setRotater(mRotater);

	mLine.startsWith(mRobot->mapToScene(mRobot->boundingRect().center()));
	mPolygon = mScene->addPolygon(mLine, QPen(Qt::black));
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
		mLine.clear();
		mScene->clear();
		mRobot = NULL;
		mPolygon = NULL;
	}
	mUi->graphicsView->setVisible(false);
	setVisible(false);
}

void D2ModelWidget::update()
{
	QWidget::update();
	drawWalls();
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
	mLine.push_back(mRobot->mapToScene(mRobot->boundingRect().center()));
	mPolygon->setPolygon(mLine);

	QPoint relativeCoords = mUi->graphicsView->mapFromScene(mRobot->pos());
	if (!mUi->graphicsView->rect().contains(relativeCoords))
		mUi->graphicsView->centerOn(mRobot);
}

void D2ModelWidget::drawWalls()
{
	QPen pen(Qt::red);
	pen.setWidth(5);
	typedef QPair<QPointF, QPointF> Wall;
	foreach (Wall const wall, mWorldModel->walls()) {
		mScene->addLine(QLineF(wall.first,wall.second), pen);
	}
}

void D2ModelWidget::drawBeep(QColor const &color)
{
	Q_UNUSED(color)
	//	mRobot->setPen(QPen(color));
}

QPolygonF const D2ModelWidget::robotBoundingPolygon(QPointF const &coord, qreal const &angle) const
{
	Q_UNUSED(angle)
	Q_UNUSED(coord)
	QRectF rect = mRobot->boundingRect();
	return QPolygonF() << rect.topLeft() << rect.topRight() << rect.bottomRight() << rect.bottomLeft();
}

void D2ModelWidget::addWall(bool on)
{
	if (!on) {
		mDrawingAction = drawingAction::none;
		mMouseClicksCount = 0;
		return;
	}

	setActiveButton(mButtonsCount - 2);

	mDrawingAction = drawingAction::wall;
}

void D2ModelWidget::setActiveButton(int active)
{
	mButtonFlags.clear();
	for (int i = 0; i < mButtonsCount; i++)
		mButtonFlags.append(false);

	mButtonFlags.replace(active, true);

	mUi->port1AddButton->setChecked(mButtonFlags.at(0));
	mUi->port2AddButton->setChecked(mButtonFlags.at(1));
	mUi->port3AddButton->setChecked(mButtonFlags.at(2));
	mUi->port4AddButton->setChecked(mButtonFlags.at(3));
	mUi->wallButton->setChecked(mButtonFlags.at(4));
}

void D2ModelWidget::clearScene()
{
	mWorldModel->clearScene();
	mRobot->clearSensors();
	mScene->clear();
	drawInitialRobot();
}

void D2ModelWidget::resetButtons()
{
	mCurrentWall.clear();
	mMouseClicksCount = 0;
	mDrawingAction = drawingAction::none;
	setActiveButton(mButtonsCount - 1);
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

QPushButton *D2ModelWidget::currentPortButton()
{
	switch (mCurrentPort){
	case inputPort::port1:
		return mUi->port1AddButton;
	case inputPort::port2:
		return mUi->port2AddButton;
	case inputPort::port3:
		return mUi->port3AddButton;
	case inputPort::port4:
		return mUi->port4AddButton;
	case inputPort::none:
		break;
	}
	return NULL;
}

void D2ModelWidget::addPort(int const port)
{
	mCurrentPort = static_cast<inputPort::InputPortEnum>(port);

	setActiveButton(port);

	mDrawingAction = drawingAction::port;

	switch (currentComboBox()->currentIndex()){
	case 0:
		mCurrentSensorType = sensorType::touchBoolean;
		break;
	case 1:
		mCurrentSensorType = sensorType::colorFull;
		break;
	case 2:
		mCurrentSensorType = sensorType::sonar;
		break;
	default:
		mCurrentSensorType = sensorType::unused;
		break;
	}
}

void D2ModelWidget::mouseClicked(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF const position = mouseEvent->scenePos();

	switch (mDrawingAction){
	case drawingAction::wall:
		mCurrentWall.append(position);
		mMouseClicksCount++;
		break;
	case drawingAction::port: {
		QPointF newpos = mRobot->mapFromScene(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
		mRobotModel->configuration().setSensor(mCurrentPort, mCurrentSensorType, newpos.toPoint(), 0); // TODO: handle direction

		reinitSensor(mCurrentPort);

		resetButtons();
	}
	break;
	case drawingAction::none:
		mMouseClicksCount = 0;
		break;
	default:
		break;
	}

	if (mMouseClicksCount >= 2)
	{
		mWorldModel->addWall(mCurrentWall.at(0), mCurrentWall.at(1));
		mCurrentWall.clear();
		mMouseClicksCount = 0;
	}
	update();
}

void D2ModelWidget::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
	Q_UNUSED(mouseEvent)
}

void D2ModelWidget::mouseMoved(QGraphicsSceneMouseEvent *mouseEvent)
{
	Q_UNUSED(mouseEvent)
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
	root.appendChild(mWorldModel->serialize(save));
	root.appendChild(mRobotModel->configuration().serialize(save));

	utils::OutFile saveFile(saveFileName);
	saveFile() << "<?xml version='1.0' encoding='utf-8'?>\n";
	saveFile() << save.toString(4);
}

void D2ModelWidget::loadWorldModel()
{
	// Loads world and robot models simultaneously.
	QString const saveFileName = QFileDialog::getOpenFileName(this, tr("Saving world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (saveFileName.isEmpty())
		return;

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
	mLine.clear();
	mPolygon->setPolygon(mLine);
}

void D2ModelWidget::removeSensor(inputPort::InputPortEnum port)
{
	// Here's the point where all interested entities are notified about sensor deletion,
	// so if this code gets broken or worked around, we'll have some almost undebuggable
	// dangling pointers in scene and in robot item. But what could possibly go wrong?
	if (mSensors[port]) {
		mRobot->removeSensor(mSensors[port]);
		mScene->removeItem(mSensors[port]);
		delete mSensors[port];
		mSensors[port] = NULL;
	}
}

void D2ModelWidget::reinitSensor(inputPort::InputPortEnum port)
{
	removeSensor(port);

	if (mRobotModel->configuration().type(port) == sensorType::unused)
		return;

	SensorItem *sensor = mRobotModel->configuration().type(port) == sensorType::sonar
			? new SonarSensorItem(*mWorldModel, mRobotModel->configuration(), port)
			: new SensorItem(mRobotModel->configuration(), port);

	mRobot->addSensor(sensor);
	mScene->addItem(sensor);
	sensor->setBasePosition(mRobot->scenePos());
	mSensors[port] = sensor;
}

void D2ModelWidget::deleteItem(QGraphicsItem *item)
{
	SensorItem * const sensor = dynamic_cast<SensorItem *>(item);
	if (!sensor)
		return;

	int const port = mSensors.indexOf(sensor);
	if (port != -1)
		removeSensor(static_cast<inputPort::InputPortEnum>(port));
}
