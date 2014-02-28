#include "editorView.h"
#include "miniMapButton.h"


using namespace qReal;

MiniMapButton::MiniMapButton(EditorView *editorView)
	: mWidgetMoved(false)
{
	this->raise();
	connect(this, SIGNAL(moveMiniMap(QPoint)), editorView, SLOT(moveMiniMap(QPoint)));
}

void MiniMapButton::mouseMoveEvent(QMouseEvent *event)
{
	changeDragState(true);
	emit moveMiniMap(event->globalPos());
}

void MiniMapButton::changeDragState(bool isDragged)
{
	mWidgetMoved = isDragged;
}

bool MiniMapButton::getDragState()
{
	return mWidgetMoved;
}
