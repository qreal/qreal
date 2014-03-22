#include "editorView.h"
#include "miniMapButton.h"

using namespace qReal;
MiniMapButton::MiniMapButton(EditorView *editorView)
	: mWidgetMoved(false)
{
	raise();
	connect(this, &MiniMapButton::moveMiniMap, editorView, &EditorView::moveMiniMap);
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
