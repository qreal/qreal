#include "hotKeyManager.h"
#include "hotKeyAction.h"

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtCore/QDebug>

using namespace qReal;

HotKeyManager* HotKeyManager::mInstance = 0;

HotKeyManager::HotKeyManager()
{
}

HotKeyManager* HotKeyManager::instance()
{
	if (mInstance == 0) {
		mInstance = new HotKeyManager();
	}
	return mInstance;
}

void HotKeyManager::setCommand(QString id, QString label, HotKeyAction *cmd)
{
	cmd->setWhatsThis(label);

	instance()->addNewShortcut(id, cmd->shortcut().toString());

	instance()->setCmd(id, cmd);
}

bool HotKeyManager::setShortcut(QString id, QKeySequence keyseq)
{
	return instance()->setScut(id, keyseq);
}

void HotKeyManager::doShortCut(QKeyEvent *keyevent)
{
	QKeySequence const keyseq = QKeySequence(keyevent->modifiers() + keyevent->key());

	instance()->findShortcut(keyseq.toString());
}

void HotKeyManager::setCmd(QString id, HotKeyAction *cmd)
{
	mCmds[id] = cmd;
}

bool HotKeyManager::setScut(QString id, QKeySequence keyseq)
{
	if (mCmds.contains(id)) {
		QString const shortcut = keyseq.toString();

		if (!mShortCuts.contains(shortcut)) {
			mShortCuts.remove(mCmds[id]->shortcut().toString());
			mShortCuts[shortcut] = id;
			mCmds[id]->setShortcut(keyseq);
			return true;
		}
	}
	return false;
}

void HotKeyManager::addNewShortcut(QString id, QString const shortcut)
{
	if (!mShortCuts.contains(shortcut)) {
		mShortCuts[shortcut] = id;
	}
}

void HotKeyManager::findShortcut(QString const shortcut)
{
	if (mShortCuts.contains(shortcut)) {
		mCmds[mShortCuts.value(shortcut)]->press();
	}
}
