#include "scene.h"
#include <QtCore/QDebug>
#include <QtCore/QPointF>

Scene::Scene(QObject * parent)
	:  QGraphicsScene(parent), mWaitLine(false), mWaitEllipse(false)
	, mWaitArch(false), mWaitRectangle(false), mCount(0)
{
	setItemIndexMethod(NoIndex);
	mEmptyRect = addRect(0, 0, 710, 600, QPen(Qt::white));
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
	setX2andY2(event);
	mLine->setBottomRight(mX2, mY2);
}

void Scene::reshapeEllipse(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mEllipse->setBottomRight(mX2, mY2);
}

void Scene::reshapeRectangle(QGraphicsSceneMouseEvent *event)
{
	setX2andY2(event);
	mRectangle->setBottomRight(mX2, mY2);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if (mWaitLine) {
		setX1andY1(event);
		mLine = new Line(mX1, mY1, mX1, mY1, NULL);
		addItem(mLine);
	} else if (mWaitEllipse) {
		setX1andY1(event);
		mEllipse = new Ellipse(mX1, mY1, mX1, mY1, NULL);
		addItem(mEllipse);
	} else if (mWaitArch && (mCount <= 2)) {
		if (mCount == 1) {
			setX1andY1(event);
		} else if (mCount == 2) {
			setX2andY2(event);
			mArch = new Arch(mX1, mY1, mX2, mY2, mEllipse);
			removeItem(mEllipse);
			addItem(mArch);
			mWaitArch = false;
		}
		mCount++;
	} else if (mWaitRectangle) {
		setX1andY1(event);
		mRectangle = new Rectangle(mX1, mY1, mX1, mY1, NULL);
		addItem(mRectangle);
	}
}

void Scene::mouseMoveEvent( QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (mWaitLine)
		reshapeLine(event);
	else if (mWaitEllipse)
		reshapeEllipse(event);
	else if (mWaitRectangle)
		reshapeRectangle(event);
	update();
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
	} else if (mWaitRectangle) {
		reshapeRectangle(event);
		mWaitRectangle = false;
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

void Scene::drawRectangle()
{
	mWaitRectangle = true;
}

void Scene::clearScene()
{
	clear();
}

