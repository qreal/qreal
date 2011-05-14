#include "contextMenuAction.h"

using namespace UML;

ContextMenuAction::ContextMenuAction(QString const &text, QObject *parent)
	: QAction(text, parent)
{
	connect(this, SIGNAL(triggered()), SLOT(trigger()));
}

void ContextMenuAction::setEventPos(QPointF const &pos)
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
