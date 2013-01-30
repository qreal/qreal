#include <QtCore/qmath.h>
#include <QtGui/QFileDialog>
#include <QtGui/QRegion>

#include "d2ModelWidget.h"
#include "ui_d2Form.h"

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
		, mMaxDrawCyclesBetweenPathElements(SettingsManager::value("drawCyclesBetweenPathElements").toInt())
		, mRobotModel(robotModel)
		, mWorldModel(worldModel)
		, mDrawingAction(drawingAction::none)
		, mMouseClicksCount(0)
		, mCurrentWall(NULL)
		, mCurrentLine(NULL)
		, mCurrentStylus(NULL)
		, mCurrentEllipse(NULL)
		, mCurrentPort(inputPort::none)
		, mCurrentSensorType(sensorType::unused)
		, mButtonsCount(8) // magic numbers are baaad, mkay?
		, mWidth(15)
		, mClearing(false)
{
	setWindowIcon(QIcon(":/icons/kcron.png"));

	initWidget();

	connectUiButtons();

	connect(mScene, SIGNAL(mousePressed(QGraphicsSceneMouseEvent *)), this, SLOT(mousePressed(QGraphicsSceneMouseEvent*)));
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

	mScene = new D2ModelScene(mUi->graphicsView);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	move(0, 0);

	mUi->penWidthSpinBox->setRange(1, 30);

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
	connect(mUi->ellipseButton, SIGNAL(toggled(bool)), this, SLOT(addEllipse(bool)));
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
	mUi->speedComboBox->setCurrentIndex(1);
}

void D2ModelWidget::changeSpeed(int curIndex)
{
	switch(curIndex){
	case 0:
		mRobotModel->setSpeedFactor(1);
		break;
	case 1:
		mRobotModel->setSpeedFactor(2);
		break;
	case 2:
		mRobotModel->setSpeedFactor(4);
		break;
	default:
		mRobotModel->setSpeedFactor(1);
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

	Rotater *rotater = new Rotater();
	rotater->setMasterItem(mRobot);
	rotater->setVisible(false);

	mRobot->setRotater(rotater);
	mRobot->setRobotModel(mRobotModel);

	mUi->graphicsView->centerOn(mRobot);
}

void D2ModelWidget::keyPressEvent(QKeyEvent *event)
{
	QWidget::keyPressEvent(event);
	if (event->matches(QKeySequence::ZoomIn)) {
		mScene->mainView()->zoomIn();
	} else if (event->matches(QKeySequence::ZoomOut)) {
		mScene->mainView()->zoomOut();
	}
}

QPointF D2ModelWidget::robotPos() const
{
	return mRobot ? mRobot->pos() : QPointF(0,0);
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

bool D2ModelWidget::isRobotOnTheGround()
{
	return mRobot && mRobot->isOnTheGround();
}

void D2ModelWidget::draw(QPointF const &newCoord, qreal angle)
{
	mRobot->setPos(newCoord);
	mRobot->setRotation(angle);

	QRectF const viewPortRect = mUi->graphicsView->mapToScene(mUi->graphicsView->viewport()->rect()).boundingRect();
	if (!viewPortRect.contains(mRobot->sceneBoundingRect().toRect())) {
		QRectF const requiredViewPort = viewPortRect.translated(mRobot->scenePos() - viewPortRect.center());
		mScene->setSceneRect(mScene->itemsBoundingRect().united(requiredViewPort));
		mUi->graphicsView->centerOn(mRobot);
	}
}

void D2ModelWidget::drawWalls()
{
	if (mDrawingAction == drawingAction::wall || mDrawingAction == drawingAction::noneWordLoad) {
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
	if (mDrawingAction == drawingAction::line
			|| mDrawingAction == drawingAction::stylus
			|| mDrawingAction == drawingAction::ellipse
			|| mDrawingAction == drawingAction::noneWordLoad) {
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
	QPointF const realRotatePoint = QPointF(mRobot->boundingRect().width()/2, mRobot->boundingRect().height()/2);
	QPointF const translationToZero = -realRotatePoint - mRobot->boundingRect().topLeft();
	QPointF const finalTranslation = coord + realRotatePoint + mRobot->boundingRect().topLeft();
	QTransform const transform = QTransform().translate(finalTranslation.x(), finalTranslation.y())
			.rotate(angle).translate(translationToZero.x(), translationToZero.y());
	return transform.map(path);
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

void D2ModelWidget::addEllipse(bool on)
{
	if (!on) {
		mDrawingAction = drawingAction::none;
		mMouseClicksCount = 0;
		return;
	}

	mDrawingAction = drawingAction::ellipse;
}

void D2ModelWidget::clearScene()
{
	mClearing = true;
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
	drawInitialRobot();
	mClearing = false;
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
	mRobotModel->configuration().setSensor(mCurrentPort, mCurrentSensorType, newpos.toPoint(), 0, true);
	reinitSensor(mCurrentPort);

	resetButtons();

}

void D2ModelWidget::reshapeWall(QGraphicsSceneMouseEvent *event)
{
	QPointF const pos = event->scenePos();
	if (mCurrentWall) {
		QPointF oldPos = mCurrentWall->end();
		mCurrentWall->setX2andY2(pos.x(), pos.y());
		if (mCurrentWall->realShape().intersects(mRobot->realBoundingRect())) {
			mCurrentWall->setX2andY2(oldPos.x(), oldPos.y());
		}
		if (event->modifiers() & Qt::ShiftModifier) {
			mCurrentWall->reshapeRectWithShift();
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
	mRobot->checkSelection();
	foreach (SensorItem *sensor, mSensors) {
		if (sensor) {
			sensor->checkSelection();
		}
	}

	QPointF const position = mouseEvent->scenePos();
	mScene->setDragMode(mDrawingAction);
	switch (mDrawingAction){
	case drawingAction::wall: {
		if (!mRobot->realBoundingRect().intersects(QRectF(position, position))) {
			mCurrentWall = new WallItem(position, position);
			mScene->removeMoveFlag(mouseEvent, mCurrentWall);
			mWorldModel->addWall(mCurrentWall);
			mMouseClicksCount++;
		}
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
	case drawingAction::ellipse: {
		mCurrentEllipse = new EllipseItem(position, position);
		mCurrentEllipse->setPen(mScene->penStyleItems(), mScene->penWidthItems(), mScene->penColorItems());
		mScene->removeMoveFlag(mouseEvent, mCurrentEllipse);
		mWorldModel->addColorField(mCurrentEllipse);
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
		if (sensor) {
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
	case drawingAction::ellipse:
		reshapeEllipse(mouseEvent);
		break;
	default:
		mScene->forMoveResize(mouseEvent, mRobot->realBoundingRect());
		break;
	}
	mScene->update();
}

void D2ModelWidget::mouseReleased(QGraphicsSceneMouseEvent *mouseEvent)
{
	mRobot->checkSelection();
	foreach (SensorItem *sensor, mSensors) {
		if (sensor) {
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
	case drawingAction::ellipse: {
		reshapeEllipse(mouseEvent);
		mCurrentEllipse = NULL;
		mMouseClicksCount = 0;
		mDrawingAction = drawingAction::none;
	}
		break;
	default:
		mScene->forReleaseResize(mouseEvent, mRobot->realBoundingRect());
		break;
	}
	mUi->wallButton->setChecked(false);
	mUi->lineButton->setChecked(false);
	mUi->stylusButton->setChecked(false);
	mUi->ellipseButton->setChecked(false);
	mScene->setMoveFlag(mouseEvent);
	mScene->update();
}

void D2ModelWidget::saveWorldModel()
{
	// Saves world and robot models simultaneously, for now.
	QString saveFileName = QFileDialog::getSaveFileName(this, tr("Saving world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (saveFileName.isEmpty()) {
		return;
	}
	if (!saveFileName.toLower().endsWith(".xml")) {
		saveFileName += ".xml";
	}

	QDomDocument save;
	QDomElement root = save.createElement("root");
	save.appendChild(root);
	root.appendChild(mWorldModel->serialize(save, QPoint(0, 0)));
	mRobotModel->serialize(save);

	utils::OutFile saveFile(saveFileName);
	saveFile() << "<?xml version='1.0' encoding='utf-8'?>\n";
	saveFile() << save.toString(4);
}

void D2ModelWidget::loadWorldModel()
{
	// Loads world and robot models simultaneously.
	QString const loadFileName = QFileDialog::getOpenFileName(this, tr("Loading world and robot model"), ".", tr("2D model saves (*.xml)"));
	if (loadFileName.isEmpty()) {
		return;
	}

	clearScene();

	QDomDocument const save = utils::xmlUtils::loadDocument(loadFileName);

	QDomNodeList const worldList = save.elementsByTagName("world");
	QDomNodeList const robotList = save.elementsByTagName("robot");
	if (worldList.count() != 1 || robotList.count() != 1) {
		// TODO: Report error
		return;
	}

	mWorldModel->deserialize(worldList.at(0).toElement());
	mRobotModel->deserialize(robotList.at(0).toElement());

	for (int i = 0; i < 4; ++i) {
		reinitSensor(static_cast<inputPort::InputPortEnum>(i));
	}

	mDrawingAction = drawingAction::noneWordLoad;
	update();
	mDrawingAction = drawingAction::none;
}

void D2ModelWidget::handleNewRobotPosition()
{
	foreach (WallItem *wall, mWorldModel->walls()) {
		if (wall->realShape().intersects(mRobot->realBoundingRect())) {
			// TODO: Find out behaviour in this case
			deleteItem(wall);
		}
	}
}

void D2ModelWidget::removeSensor(inputPort::InputPortEnum port)
{
	// Here's the point where all interested entities are notified about sensor deletion,
	// so if this code gets broken or worked around, we'll have some almost undebuggable
	// dangling pointers in scene and in robot item. But what could possibly go wrong?
	if (!mSensors[port]) {
		return;
	}

	mRobot->removeSensor(mSensors[port]);
	mScene->removeItem(mSensors[port]);
	delete mSensors[port];
	mSensors[port] = NULL;

	changeSensorType(port, sensorType::unused);
}

void D2ModelWidget::changeSensorType(inputPort::InputPortEnum const port
		, sensorType::SensorTypeEnum const type)
{
	switch (port) {
	case inputPort::port1:
		mUi->port1Box->setCurrentIndex(sensorTypeToComboBoxIndex(type));
		break;
	case inputPort::port2:
		mUi->port2Box->setCurrentIndex(sensorTypeToComboBoxIndex(type));
		break;
	case inputPort::port3:
		mUi->port3Box->setCurrentIndex(sensorTypeToComboBoxIndex(type));
		break;
	case inputPort::port4:
		mUi->port4Box->setCurrentIndex(sensorTypeToComboBoxIndex(type));
		break;
	default:
		break;
	}
}

int D2ModelWidget::sensorTypeToComboBoxIndex(sensorType::SensorTypeEnum const type)
{
	switch(type) {
	case sensorType::unused:
		return 0;
	case sensorType::touchBoolean:
		return 1;
	case sensorType::touchRaw:
		return 1;
	case sensorType::sonar:
		return 3;
	case sensorType::colorFull:
		return 2;
	case sensorType::colorRed:
		return 2;
	case sensorType::colorGreen:
		return 2;
	case sensorType::colorBlue:
		return 2;
	case sensorType::colorNone:
		return 2;
	default:
		return 0;
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

	mRobot->addSensor(sensor);
	mScene->addItem(sensor);

	sensor->addStickyItem(mRobot);

	// Setting sensor rotaters
	Rotater *rotater = new Rotater();
	rotater->setMasterItem(sensor);
	rotater->setVisible(false);
	sensor->setRotater(rotater);
	sensor->setRotation(mRobotModel->configuration().direction(port));

	sensor->setParentItem(mRobot);
	sensor->setPos(mRobot->mapFromScene(mRobotModel->configuration().position(port)));

	changeSensorType(port, mRobotModel->configuration().type(port));
	mSensors[port] = sensor;
}

void D2ModelWidget::deleteItem(QGraphicsItem *item)
{
	// TODO: Handle all cases equally
	SensorItem * const sensor = dynamic_cast<SensorItem *>(item);
	if (sensor) {
		int const port = mSensors.indexOf(sensor);
		if (port != -1) {
			removeSensor(static_cast<inputPort::InputPortEnum>(port));
		}
		return;
	}
	WallItem *wall = dynamic_cast<WallItem *>(item);
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
	foreach (QGraphicsItem *graphicsItem, listSelectedItems) {
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
	if (mClearing) {
		return;
	}
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

void D2ModelWidget::setRobotVisible(bool isVisible)
{
	if (!isVisible) {
		mRobotWasSelected = mRobot->isSelected();
	}
	mRobot->setVisible(isVisible);
	if (isVisible) {
		mRobot->setSelected(mRobotWasSelected);
	}
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
	Q_UNUSED(event)
	emit d2WasClosed();
}

void D2ModelWidget::worldWallDragged(WallItem *wall, const QPainterPath &shape
		, const QPointF &oldPos)
{
	bool const isNeedStop = shape.intersects(mRobot->realBoundingRect());
	wall->onOverlappedWithRobot(isNeedStop);
	if (wall->isDragged()) {
		if (isNeedStop) {
			wall->setPos(oldPos);
		}
		wall->setFlag(QGraphicsItem::ItemIsMovable, !isNeedStop);
	}
}
