#include "contextMenuAction.h"

ContextMenuAction::ContextMenuAction(const QString &text, QObject *parent)
	: QAction(text, parent)
{
	connect(this, SIGNAL(triggered()), SLOT(trigger()));
}

void ContextMenuAction::setEventPos(const QPointF &pos)
{
	mEventPos = pos;
}

QPointF ContextMenuAction::eventPos() const
{
	return mEventPos;
}

void ContextMenuAction::trigger()
{
	emit triggered(mEventPos);
}
