#include "miniMapShell.h"
#include "miniMapButton.h"

using namespace qReal;

MiniMapButton::MiniMapButton(MiniMapShell *parent)
	: mWidgetMoved(false)
{
	raise();
	connect(this, &MiniMapButton::moveMiniMap, parent, &MiniMapShell::replace);
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
