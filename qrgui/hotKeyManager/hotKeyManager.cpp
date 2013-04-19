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
	//label
	QList<QString> shortcuts = cmd->shortcuts();

	instance()->setCmd(id, cmd);

	if (!shortcuts.empty()) {
		foreach(QString shortcut, shortcuts) {
			instance()->addNewShortcut(id, shortcut);
		}
	}
}

bool HotKeyManager::addShortcut(QString const id, QKeySequence keyseq)
{
	return instance()->addNewShortcut(id, keyseq.toString());
}

bool HotKeyManager::addShortcut(QString const id, QKeySequence keyseq,  MouseShortcuts mouseShortcut)
{
	QString const seq = HotKeyAction::sequence(keyseq, mouseShortcut);
	return instance()->addNewShortcut(id, seq);
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
	MouseShortcuts const msc = wheelevent->delta() > 0 ? MouseWheelUp : MouseWheelDown;
	QString const seq = HotKeyAction::sequence(instance()->currentKeySeq(), msc);

	instance()->findShortcut(seq);
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

	QString const seq = HotKeyAction::sequence(instance()->currentKeySeq(), msc);

	instance()->findShortcut(seq);
}

void HotKeyManager::clearCurrentKeySeq()
{
	instance()->setCurrentKeySeq(QKeySequence(""));
}

void HotKeyManager::resetCmdShortcuts(QString const id)
{
	instance()->resetShortcuts(id);
}


void HotKeyManager::setCmd(QString const id, HotKeyAction *cmd)
{
	mCmds[id] = cmd;
}

bool HotKeyManager::addNewShortcut(QString const id, const QString shortcut)
{
	if (mCmds.contains(id)) {
		if (addShortcut(id, shortcut)) {
			mCmds[id]->addShortcut(shortcut);
			return true;
		}
	}
	return false;
}

bool HotKeyManager::addShortcut(QString const id, QString const shortcut)
{
	if (!mShortCuts.contains(shortcut)) {
		mShortCuts[shortcut] = id;
		return true;
	}
	return false;
}

void HotKeyManager::findShortcut(QString const shortcut)
{
	if (mShortCuts.contains(shortcut)) {
		mCmds[mShortCuts.value(shortcut)]->press();
	}
}

void HotKeyManager::setCurrentKeySeq(QKeySequence keyseq)
{
	mCurrentKeySeq = keyseq;
}

QKeySequence HotKeyManager::currentKeySeq()
{
	return mCurrentKeySeq;
}

void HotKeyManager::resetShortcuts(const QString id)
{
	QList<QString> shortcuts = mCmds.value(id)->shortcuts();

	foreach(QString shortcut, shortcuts) {
		mShortCuts.remove(shortcut);
	}

	mCmds.value(id)->resetShortcuts();
}
