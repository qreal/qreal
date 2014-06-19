#pragma once

#include <QtWidgets/QGraphicsScene>

#include "toolController.h"

namespace qReal
{
namespace widgetsEdit
{

int const SCENE_WIDTH = 2000;
int const SCENE_HEIGHT = 2000;

class EditorScene : public QGraphicsScene
{
	Q_OBJECT

public:
	EditorScene(ToolController *controller, QObject *parent = 0);

protected:
	virtual void keyPressEvent(QKeyEvent *event);

private:
	ToolController *mController;
};

}
}
