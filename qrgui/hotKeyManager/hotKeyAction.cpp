#include "hotKeyManager.h"


#include <QtWidgets/QAction>
#include <QtCore/QDebug>

using namespace qReal;

HotKeyAction::HotKeyAction(QObject *parent)
	: QAction(parent)
{
}

void HotKeyAction::press()
{
	emit pressed();
}
void HotKeyAction::resetMouseShortcuts()
{
	mMouseShortcuts.clear();
}

void HotKeyAction::addNewMouseShortcut(QString const keyseq, MouseShortcuts msc)
{
	mMouseShortcuts.append(QPair<QString, MouseShortcuts> (keyseq, msc));
}

QList<QPair<QString, MouseShortcuts> > HotKeyAction::mouseShortcuts()
{
	return mMouseShortcuts;
}

