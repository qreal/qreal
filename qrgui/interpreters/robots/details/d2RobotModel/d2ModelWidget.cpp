#include "d2ModelWidget.h"
#include "ui_d2Form.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

D2ModelWidget::D2ModelWidget(IConfigurableRobotModel *robotModel, WorldModel *worldModel, QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::D2Form)
	, mScene(NULL)
	, mRobot(NULL)
	, mPolygon(NULL)
	, mRobotModel(robotModel)
	, mWorldDrawer(worldModel)
	, mWorldModel(worldModel)
	, mDrawingAction(drawingAction::none)
	, mMouseClicksCount(0)
	, mCurrentPort(inputPort::none)
	, mButtonsCount(5) // magic numbers are baaad, mkay?
{
	mUi->setupUi(this);
	mScene = new D2ModelScene(mUi->graphicsView);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	move(0, 0);

	connectUiButtons();

	connect(mScene, SIGNAL(mouseClicked(QGraphicsSceneMouseEvent *)), this, SLOT(mouseClicked(QGraphicsSceneMouseEvent *)));
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

	connect(&mPortsMapper, SIGNAL(mapped(int)), this, SLOT(addPort(int)));
}

void D2ModelWidget::init()
{
	if (!isHidden())
		return;

	drawInitialRobot();

	//	QGraphicsRectItem *wheel1 = new QGraphicsRectItem(robotWidth - widthBigWheel, 0 - heightBigWheel, widthBigWheel, heightBigWheel, mRobot);
	//	wheel1->setBrush(QBrush(Qt::gray));
	//	QGraphicsRectItem *wheel2 = new QGraphicsRectItem(robotWidth - widthBigWheel, robotHeight, widthBigWheel, heightBigWheel, mRobot);
	//	wheel2->setBrush(QBrush(Qt::gray));
	//	QGraphicsRectItem *wheel3 = new QGraphicsRectItem(0 - widthSmallWheel / 2, robotHeight / 2 - heightSmallWheel / 2, widthSmallWheel, heightSmallWheel, mRobot);
	//	wheel3->setBrush(QBrush(Qt::gray));

	mUi->graphicsView->show();
	show();
	update();
}

void D2ModelWidget::drawInitialRobot()
{
	mRobot = new RobotItem();
	mScene->addItem(mRobot);
	mLine.startsWith(mRobot->mapToScene(mRobot->boundingRect().center()));
	mPolygon = mScene->addPolygon(mLine, QPen(Qt::black));
	mUi->graphicsView->centerOn(mRobot);
}

QPointF D2ModelWidget::robotPos()
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
//	mRobot->setPen(QPen(color));
}

QPolygonF const D2ModelWidget::robotBoundingPolygon(QPointF const &coord, qreal const &angle) const
{
	Q_UNUSED(angle)
	return QPolygonF() << coord;
}

void D2ModelWidget::addWall(bool on)
{
	if (!on) {
		mDrawingAction = drawingAction::none;
		mMouseClicksCount = 0;
		return;
	}

	setActiveButton(4);

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
	mScene->clear();
	drawInitialRobot();
}

void D2ModelWidget::resetButtons()
{
	mCurrentWall.clear();
	mMouseClicksCount = 0;

	mUi->wallButton->setChecked(false);
}

void D2ModelWidget::addPort(int const port)
{
	mCurrentPort = static_cast<inputPort::InputPortEnum>(port);

	setActiveButton(static_cast<int>(mCurrentPort));
}

void D2ModelWidget::mouseClicked(QGraphicsSceneMouseEvent *mouseEvent)
{
	QPointF const position = mouseEvent->scenePos();

	switch (mDrawingAction){
	case drawingAction::wall:
		mCurrentWall.append(position);
		mMouseClicksCount++;
		break;
	case drawingAction::port:
		mMouseClicksCount = 0;
		// add port
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
