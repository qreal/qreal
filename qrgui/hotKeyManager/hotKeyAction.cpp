#include "hotKeyAction.h"

#include <QtWidgets/QAction>
#include <QtCore/QDebug>

HotKeyAction::HotKeyAction(QObject *parent)
	: QAction(parent)
{
}

void HotKeyAction::press()
{
	emit pressed();
}
