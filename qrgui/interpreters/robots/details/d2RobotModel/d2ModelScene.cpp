#include "d2ModelScene.h"

#include <QGraphicsSceneMouseEvent>
#include <QDebug>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;

D2ModelScene::D2ModelScene(QObject *parent) : QGraphicsScene(parent)
{
	setItemIndexMethod(NoIndex);
}

D2ModelScene::~D2ModelScene()
{

}

void D2ModelScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mousePressEvent(mouseEvent);
	emit mouseClicked(mouseEvent->scenePos());
}
