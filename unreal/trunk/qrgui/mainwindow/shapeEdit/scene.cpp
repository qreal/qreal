#include "scene.h"
#include <QDebug>
#include <QPointF>

Scene::Scene(QObject * parent)
	:  QGraphicsScene(parent), mWaitLine(false), mWaitEllipse(false), mWaitArch(false), mCount(0)
{
	setItemIndexMethod(NoIndex);
	addRect(0, 0, 710, 600, QPen(Qt::white));
}

void Scene::setX1andY1(QGraphicsSceneMouseEvent *event)
{
	mX1 = event->scenePos().x();
	mY1 = event->scenePos().y();
}

void Scene::setX2andY2(QGraphicsSceneMouseEvent *event)
{
	mX2 = event->scenePos().x();
	mY2 = event->scenePos().y();
}

void Scene::reshapeLine(QGraphicsSceneMouseEvent *event)
{
	removeItem(mLine);
	setX2andY2(event);
	mLine = addLine(mX1, mY1, mX2, mY2, QPen(Qt::red));
}

void Scene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	removeItem(mEllipse);
	setX2andY2(event);
	if(mX2 > mX1) {
		if (mY2 > mY1)
			mEllipse = addEllipse(mX1, mY1, mX2 - mX1, mY2 - mY1, QPen(Qt::green));
		else
			mEllipse = addEllipse(mX1, mY2, mX2 - mX1, mY1 - mY2, QPen(Qt::green));
	} else {
		if (mY2 > mY1)
			mEllipse = addEllipse(mX2, mY1, mX1 - mX2, mY2 - mY1, QPen(Qt::green));
		else
			mEllipse = addEllipse(mX2, mY2, mX1 - mX2, mY1 - mY2, QPen(Qt::green));
	}
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if (mWaitLine) {
		setX1andY1(event);
		mLine = addLine(0, 0, 0, 0, QPen(Qt::red));
	} else if (mWaitEllipse) {
		setX1andY1(event);
		mEllipse = addEllipse(0, 0, 0, 0, QPen(Qt::green));
	} else if (mWaitArch && (mCount <= 2)) {
		if (mCount == 1) {
			setX1andY1(event);
		} else if (mCount == 2) {
			setX2andY2(event);
			mEllipse->setPen(QPen(Qt::white));
			mArch = new Arch(mX1, mY1, mX2, mY2, mEllipse);
			addItem(mArch);
			mWaitArch = false;
		}
		mCount++;
	}
}

void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (mWaitLine)
		reshapeLine(event);
	else if (mWaitEllipse)
		reshapeEllipse(event);
}

void Scene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	QGraphicsScene::mouseReleaseEvent(event);
	if (mWaitLine) {
		reshapeLine(event);
		mWaitLine = false;
	} else if (mWaitEllipse) {
		reshapeEllipse(event);
		mWaitEllipse = false;
		qDebug() << "ellipse: " << "X1= " << mX1 << " Y1= " << mY1 << " X2= " << mX2 << " Y2= " << mY2;
	}
}

void Scene::drawLine()
{
	mWaitLine = true;
}

void Scene::drawEllipse()
{
	mWaitEllipse = true;
}
void Scene::drawArc()
{
	mWaitEllipse = true;
	mWaitArch = true;
	mCount = 1;
}
void Scene::clearScene()
{
	clear();
}

