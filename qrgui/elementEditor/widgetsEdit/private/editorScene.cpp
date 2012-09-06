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
	if (event->key() == Qt::Key_Delete && event->modifiers() == Qt::NoModifier) {
		mController->removeChild(mController->selectedItem());
		event->accept();
		return;
	}

	if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Left ||
		event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {

		int const delta = event->modifiers() == Qt::ControlModifier ? 1 : 5;
		int dx = 0;
		int dy = 0;

		switch(event->key()) {
		case Qt::Key_Right:
			dx = delta;
			break;
		case Qt::Key_Left:
			dx = -delta;
			break;
		case Qt::Key_Up:
			dy = -delta;
			break;
		case Qt::Key_Down:
			dy = delta;
			break;
		}

		mController->move(dx, dy);

		event->accept();
		return;
	}

	event->ignore();
}
