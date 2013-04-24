#include "hotKeyManager.h"

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtGui/QActionEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>

using namespace qReal;

HotKeyManager* HotKeyManager::mInstance = 0;

HotKeyManager::HotKeyManager()
	: mCurrentModifer("")
{
}

HotKeyManager* HotKeyManager::instance()
{
	if (mInstance == 0) {
		mInstance = new HotKeyManager();
	}
	return mInstance;
}

void HotKeyManager::setCommand(QString const id, QString const label, QAction *cmd)
{
	cmd->setWhatsThis(label);

	instance()->registerCommand(id, cmd);
}

void HotKeyManager::delCommand(QString const id)
{
	resetCmdShortcuts(id);
	instance()->delCmd(id);
}

bool HotKeyManager::setShortcut(QString const id, QKeySequence const keyseq)
{
	return instance()->registerShortcut(id, keyseq);
}

bool HotKeyManager::setShortcut(QString const id, QKeySequence const mod, MouseButtons mb)
{
	return instance()->registerShortcut(id, mod, mb);
}

void HotKeyManager::resetCmdShortcuts(QString const id)
{
	instance()->resetShortcuts(id);
}

void HotKeyManager::resetAllCmdsShortcuts()
{
	instance()->resetAllShortcuts();
}

void HotKeyManager::doShortcut(QEvent *event)
{
	MouseButtons mb = None;
	switch(event->type()) {
		case QEvent::Wheel: {
			mb = static_cast<QWheelEvent *>(event)->delta() > 0 ? MouseWU : MouseWD;
			break;
		}
		case QEvent::MouseButtonPress: {
			switch(static_cast<QMouseEvent *>(event)->button()) {
				case Qt::RightButton:
					mb = MouseRB;
					break;
				case Qt::LeftButton:
					mb = MouseLB;
					break;
				case Qt::MiddleButton:
					mb = MouseMB;
					break;
				default:
					return;
			}
			break;
		}
		default: {
			return;
		}
	}

	instance()->findShortcut(sequence(instance()->currentModifier(), mb));
}

void HotKeyManager::setCurrentModifier(QString const mod)
{
	instance()->setCurrMod(mod);
}

QHash<QString, QAction *> HotKeyManager::commands()
{
	return instance()->cmds();
}

QHash<QString, QString> HotKeyManager::cmdsShortcuts()
{
	return instance()->shortcuts();
}

void HotKeyManager::registerCommand(QString const id, QAction *cmd)
{
	QList<QKeySequence> shortcuts = cmd->shortcuts();

	foreach(QKeySequence shortcut, shortcuts) {
		instance()->registerShortcut(id, shortcut.toString());
	}

	mCmds[id] = cmd;
}

bool HotKeyManager::registerShortcut(QString const id, QKeySequence const keyseq)
{
	if (mCmds.contains(id)) {
		QString const shortcut = keyseq.toString();

		if (!mShortcuts.contains(shortcut)) {
			mShortcuts[shortcut] = id;
			QList<QKeySequence> seqs = mCmds[id]->shortcuts();
			seqs.append(keyseq);
			mCmds[id]->setShortcuts(seqs);
			return true;
		}
	}
	return false;
}

bool HotKeyManager::registerShortcut(QString const id, QKeySequence const mod, MouseButtons mb)
{
	if (mCmds.contains(id)) {
		QString const shortcut = sequence(mod.toString(), mb);

		if (!mShortcuts.contains(shortcut)) {
			mShortcuts[shortcut] = id;
			return true;
		}
	}

	return false;
}

void HotKeyManager::registerShortcut(QString const id, QString const shortcut)
{
	if (!mShortcuts.contains(shortcut)) {
		mShortcuts[shortcut] = id;
	}
}

void HotKeyManager::findShortcut(QString const shortcut)
{
	if (mShortcuts.contains(shortcut) && mCmds[mShortcuts.value(shortcut)]->parentWidget()->isActiveWindow()) {
		mCmds[mShortcuts.value(shortcut)]->trigger();
	}
}

QString HotKeyManager::sequence(QString const mod, MouseButtons mb)
{
	QString seq = mod;

	switch(mb) {
	case MouseLB:
		seq += "MouseLB";
		break;
	case MouseRB:
		seq += "MouseRB";
		break;
	case MouseMB:
		seq += "MouseMB";
		break;
	case MouseWU:
		seq += "MouseWU";
		break;
	case MouseWD:
		seq += "MouseWD";
	}

	return seq;
}

void HotKeyManager::setCurrMod(QString const mod)
{
	mCurrentModifer = mod;
}

QString HotKeyManager::currentModifier()
{
	return mCurrentModifer;
}

void HotKeyManager::resetShortcuts(QString const id)
{
	if (mCmds.contains(id)) {
		QList<QString> shortcuts = mShortcuts.keys(id);

		foreach(QString shortcut, shortcuts) {
			mShortcuts.remove(shortcut);
		}

		mCmds[id]->setShortcuts(QList<QKeySequence>());
	}
}

void HotKeyManager::resetAllShortcuts()
{
	QList<QAction *> cmds = mCmds.values();

	foreach(QAction *cmd, cmds) {
		cmd->setShortcuts(QList<QKeySequence>());
	}

	mShortcuts.clear();
}

void HotKeyManager::delCmd(QString const id)
{
	if (mCmds.contains(id)) {
		mCmds.remove(id);
	}
}

QHash<QString, QAction *> HotKeyManager::cmds()
{
	return mCmds;
}

QHash<QString, QString> HotKeyManager::shortcuts()
{
	return mShortcuts;
}
