#include <QtGui/QKeyEvent>

#include "editorScene.h"

using namespace qReal::widgetsEdit;

EditorScene::EditorScene(ToolController *controller, QObject *parent)
	: QGraphicsScene(QRectF(0, 0, SCENE_WIDTH, SCENE_HEIGHT), parent)
	, mController(controller)
{
}

void EditorScene::keyPressEvent(QKeyEvent *event)
{
	QGraphicsScene::keyPressEvent(event);
	mController->processKeyEvent(event);
}
