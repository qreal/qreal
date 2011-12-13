#include "d2ModelScene.h"

#include <QtGui/QGraphicsSceneMouseEvent>
#include <QtGui/QKeyEvent>

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

D2ModelScene::D2ModelScene(AbstractView *view, QObject *parent)
		: AbstractScene(view, parent)
{
	mFirstPenWidth = 6;
	mSizeEmptyRectX = 1000;
	mSizeEmptyRectY = 1000;
	setEmptyRect(-500, -500, mSizeEmptyRectX, mSizeEmptyRectY);
	setItemIndexMethod(NoIndex);
	setEmptyPenBrushItems();
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

void D2ModelScene::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete && (selectedItems().size() > 0)) {
		foreach(QGraphicsItem* item, selectedItems()) {
			emit itemDeleted(item);
		}
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}
