#include "robotDrawer.h"
#include "ui_d2Form.h"
#include <QtCore/QDebug>
using namespace qReal::interpreters::robots;
using namespace details::d2Model;

RobotDrawer::RobotDrawer(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::D2Form)
	, mRobot(NULL)
	, mPolygon(NULL)
{
	mUi->setupUi(this);
	mScene = new QGraphicsScene(mUi->graphicsView);
	mUi->graphicsView->setScene(mScene);
	move(0, 0);
	mScene->addRect(-500, -500, 1000, 1000, QPen(Qt::NoPen), QBrush(Qt::NoBrush));
}

RobotDrawer::~RobotDrawer()
{
	delete mPolygon;
	delete mRobot;
	delete mScene;
	delete mUi;
}

void RobotDrawer::init()
{
	mLine.startsWith(QPointF(0, 0));
	mPolygon = mScene->addPolygon(mLine, QPen(Qt::black));
	QImage image = QImage(":/icons/robot.png");
	mRobot = mScene->addRect(0, 0, robotWidth, robotHeight, QPen(Qt::green), QBrush(image));
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

void RobotDrawer::close()
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

void RobotDrawer::draw(QPointF newCoord, qreal angle, QPointF dPoint)
{
	mRobot->setPos(newCoord);
	mRobot->setTransform(QTransform().translate(dPoint.x(), dPoint.y()).rotate(angle).translate(-dPoint.x(), -dPoint.y()));
	mLine.push_back(mRobot->scenePos());
	mPolygon->setPolygon(mLine);

	QPoint relativeCoords = mUi->graphicsView->mapFromScene(mRobot->pos());
	if (!mUi->graphicsView->rect().contains(relativeCoords))
		mUi->graphicsView->centerOn(mRobot);
}

void RobotDrawer::drawBeep(QColor const &color)
{
	mRobot->setPen(QPen(color));
}
