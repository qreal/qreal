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

void HotKeyManager::setCommand(QString id, QString label, QAction *cmd)
{
	cmd->setWhatsThis(label);

	instance()->setCmd(id, cmd);
}

void HotKeyManager::delCommand(QString id)
{
	resetCmdShortcuts(id);
	instance()->delCmd(id);
}

bool HotKeyManager::setShortcut(QString id, QKeySequence keyseq)
{
	return instance()->setScut(id, keyseq);
}

bool HotKeyManager::setShortcut(QString id, Qt::Modifier mod, MouseButtons mb)
{
	return instance()->setScut(id, mod, mb);
}

void HotKeyManager::resetCmdShortcuts(QString id)
{
	instance()->resetShortcuts(id);
}

void HotKeyManager::resetAllCmdsShortcuts()
{
	instance()->resetAllShortcuts();
}

void HotKeyManager::doShortcut(QEvent *event)
{
	MouseButtons mb = MouseRB;
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
			default:
				break;
			}
		}
		default: {
		}
	}

	instance()->findShortcut(instance()->sequence(instance()->currentModifier(), mb));
}

void HotKeyManager::setCurrentModifier(QString mod)
{
	instance()->setCurrMod(mod);
}


void HotKeyManager::setCmd(QString id, QAction *cmd)
{
	QList<QKeySequence> shortcuts = cmd->shortcuts();

	foreach(QKeySequence shortcut, shortcuts) {
		instance()->addNewShortcut(id, shortcut.toString());
	}

	mCmds[id] = cmd;
}

bool HotKeyManager::setScut(QString id, QKeySequence keyseq)
{
	if (mCmds.contains(id)) {
		QString const shortcut = keyseq.toString();

		if (!mShortCuts.contains(shortcut)) {
			mShortCuts[shortcut] = id;
			QList<QKeySequence> seqs = mCmds[id]->shortcuts();
			seqs.append(keyseq);
			mCmds[id]->setShortcuts(seqs);
			return true;
		}
	}
	return false;
}

bool HotKeyManager::setScut(QString id, Qt::Modifier mod, MouseButtons mb)
{
	if (mCmds.contains(id)) {
		QString const shortcut = sequence(QKeySequence(mod).toString(), mb);

		if (!mShortCuts.contains(shortcut)) {
			mShortCuts[shortcut] = id;
			return true;
		}
	}

	return false;
}

void HotKeyManager::addNewShortcut(QString id, QString const shortcut)
{
	qDebug() << shortcut;
	if (!mShortCuts.contains(shortcut)) {
		mShortCuts[shortcut] = id;
	}
}

void HotKeyManager::findShortcut(QString const shortcut)
{
	if (mShortCuts.contains(shortcut)) {
		mCmds[mShortCuts.value(shortcut)]->trigger();
	}
}

QString HotKeyManager::sequence(QString mod, MouseButtons mb)
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

void HotKeyManager::setCurrMod(QString mod)
{
	mCurrentModifer = mod;
}

QString HotKeyManager::currentModifier()
{
	return mCurrentModifer;
}

void HotKeyManager::resetShortcuts(QString id)
{
	if (mCmds.contains(id)) {
		QList<QString> shortcuts = mShortCuts.keys(id);

		foreach(QString shortcut, shortcuts) {
			mShortCuts.remove(shortcut);
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

	mShortCuts.clear();
}

void HotKeyManager::delCmd(QString id)
{
	if (mCmds.contains(id)) {
		mCmds.remove(id);
	}
}
