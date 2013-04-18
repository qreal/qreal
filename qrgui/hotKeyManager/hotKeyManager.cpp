#include "hotKeyManager.h"
#include "hotKeyAction.h"

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtCore/QDebug>
#include <QtCore/QList>

using namespace qReal;

HotKeyManager* HotKeyManager::mInstance = 0;

HotKeyManager::HotKeyManager()
	: mCurrentKeySeq("")
{
}

HotKeyManager* HotKeyManager::instance()
{
	if (mInstance == 0) {
		mInstance = new HotKeyManager();
	}
	return mInstance;
}

void HotKeyManager::setCommand(QString const id, const QString label, HotKeyAction *cmd)
{
	cmd->setWhatsThis(label);

	QList<QKeySequence> shortcuts = cmd->shortcuts();

	if (!shortcuts.empty()) {
		foreach(QKeySequence shortcut, shortcuts) {
			instance()->addNewShortcut(id, shortcut.toString());
		}
	}

	instance()->setCmd(id, cmd);
}

bool HotKeyManager::addShortcut(QString const id, QKeySequence keyseq)
{
	return instance()->addNewShortcut(id, keyseq);
}

bool HotKeyManager::addShortcut(QString const id, QKeySequence keyseq,  MouseShortcuts mouseShortcut)
{
	return instance()->addNewMouseShortcut(id, keyseq.toString(), mouseShortcut);
}

void HotKeyManager::doShortcut(QKeyEvent *keyevent)
{
	QKeySequence keyseq;

	switch(keyevent->key()) {
		case 16777248: //Shift modifier
		case 16777249: //Ctrl modifier
		case 16777251: //Alt modifier
			keyseq = QKeySequence(keyevent->modifiers());
			break;
		default:
			keyseq = QKeySequence(keyevent->modifiers() + keyevent->key());
	}

	instance()->setCurrentKeySeq(keyseq);

	instance()->findShortcut(keyseq.toString());
}

void HotKeyManager::doShortcut(QWheelEvent *wheelevent)
{
	MouseShortcuts msc = wheelevent->delta() > 0 ? MouseWheelUp : MouseWheelDown;

	instance()->findShortcut(msc);
}

void HotKeyManager::doShortcut(QMouseEvent *mouseevent)
{
	MouseShortcuts msc;

	switch(mouseevent->button()) {
		case Qt::RightButton:
			msc = MouseRBClick;
			break;
		case Qt::LeftButton:
			msc = MouseLBClick;
			break;
		case Qt::MidButton:
			msc = MouseMBClick;
			break;
		default:
			return;
	}

	instance()->findShortcut(msc);
}

void HotKeyManager::clearCurrentKeySeq()
{
	instance()->setCurrentKeySeq(QKeySequence(""));
}

void HotKeyManager::resetShortcuts(QString const id)
{
	instance()->resetKeyboardShortcuts(id);

	instance()->resetMouseShortcuts(id);
}

void HotKeyManager::setCmd(QString const id, HotKeyAction *cmd)
{
	mCmds[id] = cmd;
}

bool HotKeyManager::addNewShortcut(QString const id, QKeySequence keyseq)
{
	if (mCmds.contains(id)) {
		if (addNewShortcut(id, keyseq.toString())) {
			QList<QKeySequence> list = mCmds[id]->shortcuts();
			list.append(keyseq);
			mCmds[id]->setShortcuts(list);
			return true;
		}
	}
	return false;
}

bool HotKeyManager::addNewShortcut(QString const id, QString const shortcut)
{
	if (!mShortCuts.contains(shortcut)) {
		mShortCuts[shortcut] = id;
		return true;
	}
	return false;
}

bool HotKeyManager::addNewMouseShortcut(const QString id, const QString keyseq, MouseShortcuts mouseShortcut)
{
	if (mCmds.contains(id)) {
		if (mMouseShortcuts.contains(keyseq)) {
			QHash<MouseShortcuts, QString> hash = mMouseShortcuts.value(keyseq);

			if (!hash.contains(mouseShortcut)) {
				hash[mouseShortcut] = id;
				mCmds.value(id)->addNewMouseShortcut(keyseq, mouseShortcut);
			} else {
				if (hash.value(mouseShortcut) != id) {
					return false;
				}
			}
			return true;
		} else {
			mMouseShortcuts[keyseq] = QHash<MouseShortcuts, QString> ();
			mMouseShortcuts[keyseq].insert(mouseShortcut, id);
			mCmds.value(id)->addNewMouseShortcut(keyseq, mouseShortcut);
			return true;
		}
	}
	return false;
}

void HotKeyManager::findShortcut(QString const shortcut)
{
	if (mShortCuts.contains(shortcut)) {
		mCmds[mShortCuts.value(shortcut)]->press();
	}
}

void HotKeyManager::findShortcut(MouseShortcuts msc)
{
	if (mMouseShortcuts.contains(mCurrentKeySeq)) {
		QHash<MouseShortcuts, QString> hash = mMouseShortcuts.value(mCurrentKeySeq);

		if (hash.contains(msc)) {
			mCmds[hash.value(msc)]->press();
		}
	}
}

void HotKeyManager::setCurrentKeySeq(QKeySequence keyseq)
{
	mCurrentKeySeq = keyseq;
}

void HotKeyManager::resetKeyboardShortcuts(QString const id)
{
	QList<QKeySequence> kbShortcuts = mCmds.value(id)->shortcuts();

	foreach(QKeySequence keyseq, kbShortcuts) {
		mShortCuts.remove(keyseq);
	}
	mCmds.value(id)->setShortcuts(QList<QKeySequence>());
}

void HotKeyManager::resetMouseShortcuts(QString const id)
{
	typedef QPair<QString, MouseShortcuts> msShortcut;
	QList<QPair<QString, MouseShortcuts> > msShortcuts = mCmds.value(id)->mouseShortcuts();

	foreach(msShortcut shorcut, msShortcuts) {
		mMouseShortcuts[shorcut.first].remove(shorcut.second);
		if (mMouseShortcuts.value(shorcut.first).empty()) {
			mMouseShortcuts.remove(shorcut.first);
		}
	}

	mCmds.value(id)->resetMouseShortcuts();
}
