#include "hotKeyManager.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtGui/QKeySequence>

using namespace qReal;

HotKeyAction::HotKeyAction()
{
}

void HotKeyAction::press()
{
	emit pressed();
}

void HotKeyAction::addShortcut(QString const shortcut)
{
	mShortcuts.append(shortcut);
}

void HotKeyAction::addShortcut(QKeySequence keyseq)
{
	mShortcuts.append(keyseq.toString());
}

void HotKeyAction::addShortcut(QKeySequence keyseq, MouseShortcuts mouseShortcut)
{
	mShortcuts.append(sequence(keyseq,mouseShortcut));
}

QList<QString> HotKeyAction::shortcuts()
{
	return mShortcuts;
}

QString HotKeyAction::sequence(QKeySequence keyseq, MouseShortcuts msc)
{
	switch(msc) {
		case MouseLBClick:
			return keyseq.toString() + "+" + "LBC";
		case MouseRBClick:
			return keyseq.toString() + "+" + "RBC";
		case MouseMBClick:
			return keyseq.toString() + "+" + "MBC";
		case MouseWheelUp:
			return keyseq.toString() + "+" + "WUP";
		case MouseWheelDown:
			return keyseq.toString() + "+" + "WDN";
	}
}

QString HotKeyAction::description()
{
	return mDescription;
}

void HotKeyAction::setDescription(const QString descr)
{
	mDescription = descr;
}

void HotKeyAction::resetShortcuts()
{
	mShortcuts.clear();
}


