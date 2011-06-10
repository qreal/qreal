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
{
	mUi->setupUi(this);
	mScene = new D2ModelScene(mUi->graphicsView);
	mUi->graphicsView->setScene(mScene);
	mUi->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	move(0, 0);
	mScene->addRect(-500, -500, 1000, 1000, QPen(Qt::NoPen), QBrush(Qt::NoBrush));

	connect(mUi->wallButton, SIGNAL(toggled(bool)), this, SLOT(addWall(bool)));
	connect(mScene, SIGNAL(mouseClicked(QPointF)), this, SLOT(mouseClicked(QPointF)));
}

D2ModelWidget::~D2ModelWidget()
{
	delete mPolygon;
	delete mRobot;
	delete mScene;
	delete mUi;
}

void D2ModelWidget::init()
{
	QImage image = QImage(":/icons/robot.png");
	mRobot = mScene->addRect(0, 0, robotWidth, robotHeight, QPen(Qt::green), QBrush(image));
	mLine.startsWith(mRobot->mapToScene(mRobot->boundingRect().center()));
	mPolygon = mScene->addPolygon(mLine, QPen(Qt::black));
	//	QGraphicsRectItem *wheel1 = new QGraphicsRectItem(robotWidth - widthBigWheel, 0 - heightBigWheel, widthBigWheel, heightBigWheel, mRobot);
	//	wheel1->setBrush(QBrush(Qt::gray));
	//	QGraphicsRectItem *wheel2 = new QGraphicsRectItem(robotWidth - widthBigWheel, robotHeight, widthBigWheel, heightBigWheel, mRobot);
	//	wheel2->setBrush(QBrush(Qt::gray));
	//	QGraphicsRectItem *wheel3 = new QGraphicsRectItem(0 - widthSmallWheel / 2, robotHeight / 2 - heightSmallWheel / 2, widthSmallWheel, heightSmallWheel, mRobot);
	//	wheel3->setBrush(QBrush(Qt::gray));
	mUi->graphicsView->centerOn(mRobot);
	mUi->graphicsView->show();
	show();
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

void D2ModelWidget::draw(QPointF newCoord, qreal angle, QPointF dPoint)
{
	mRobot->setPos(newCoord);
	mRobot->setTransform(QTransform().translate(dPoint.x(), dPoint.y()).rotate(angle).translate(-dPoint.x(), -dPoint.y()));
	mLine.push_back(mRobot->mapToScene(mRobot->boundingRect().center()));
	mPolygon->setPolygon(mLine);

	QPoint relativeCoords = mUi->graphicsView->mapFromScene(mRobot->pos());
	if (!mUi->graphicsView->rect().contains(relativeCoords))
		mUi->graphicsView->centerOn(mRobot);
	drawWalls();
}

void D2ModelWidget::drawWalls()
{
	typedef QPair<QPointF, QPointF> Wall;
	foreach (Wall const wall, mWorldModel->walls()) {
		mScene->addLine(QLineF(wall.first,wall.second));
	}
}

void D2ModelWidget::drawBeep(QColor const &color)
{
	mRobot->setPen(QPen(color));
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
		return;
	}

	mDrawingAction = drawingAction::wall;
}

void D2ModelWidget::mouseClicked(QPointF const &position)
{
	qDebug() << "clicked at" << position;

	switch (mDrawingAction){
	case drawingAction::wall:
		mCurrentWall.append(position);
		mMouseClicksCount++;
		break;
	case drawingAction::none:
		mMouseClicksCount = 0;
	default:
		break;
	}

	if (mMouseClicksCount >= 2)
	{
		mWorldModel->addWall(mCurrentWall.at(0), mCurrentWall.at(1));
		mCurrentWall.clear();
		mMouseClicksCount = 0;
	}
}
