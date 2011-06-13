#include "d2ModelScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

D2ModelScene::D2ModelScene(QObject *parent) : QGraphicsScene(parent)
{
	addRect(-500, -500, 1000, 1000, QPen(Qt::NoPen), QBrush(Qt::NoBrush));
	setItemIndexMethod(NoIndex);
//	clear();
}

D2ModelScene::~D2ModelScene()
{

}

void D2ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mousePressEvent(mouseEvent);
	emit mouseClicked(mouseEvent);
}

void D2ModelScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	emit mouseReleased(mouseEvent);
}

void D2ModelScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	emit mouseMoved(mouseEvent);
}

