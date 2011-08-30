#pragma once

#include "../../view/editorViewScene.h"
#include "../details/diffState.h"

namespace qReal
{
namespace diffManager
{
namespace diffView
{

class DiffScene : public EditorViewScene
{
public:
	DiffScene(QObject *parent = 0);
	void initForDiff();
	void highlight(qReal::Id const &id, details::DiffState state);
	void hint(qReal::Id const &graphicalId);

protected:
	virtual void dragEnterEvent( QGraphicsSceneDragDropEvent *event);
	virtual void dragMoveEvent( QGraphicsSceneDragDropEvent *event);
	virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent *event);
	virtual void dropEvent ( QGraphicsSceneDragDropEvent *event);

	virtual void keyPressEvent( QKeyEvent *event);

	virtual void mousePressEvent( QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);

	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event);

private:
	QColor mAddedRemovedColor;
	QColor mModifiedColor;
	QColor mHintColor;

	void highlight(UML::Element *elem, QColor const &color);
};

}
}
}
