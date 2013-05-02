#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtGui/QActionEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>

#include "hotKeyManager.h"

using namespace qReal;

HotKeyManager::HotKeyManager()
	: mCurrentModifer("")
{
}

HotKeyManager& HotKeyManager::instance()
{
	static HotKeyManager instance;
	return instance;
}

void HotKeyManager::setCommand(const QString &id, const QString &label, QAction *command)
{
	command->setWhatsThis(label);

	instance().registerCommand(id, command);
}

void HotKeyManager::deleteCommand(QString const &id)
{
	resetShortcuts(id);
	instance().deleteCommandPrivate(id);
}

bool HotKeyManager::setShortcut(QString const &id, QKeySequence const &keyseq)
{
	return instance().registerShortcut(id, keyseq);
}

bool HotKeyManager::setShortcut(QString const &id, QKeySequence const &modifier, MouseButtons mousebutton)
{
	return instance().registerShortcut(id, modifier, mousebutton);
}

void HotKeyManager::resetShortcuts(QString const &id)
{
	instance().resetShortcutsPrivate(id);
}

void HotKeyManager::resetAllShortcuts()
{
	instance().resetAllShortcutsPrivate();
}

void HotKeyManager::deleteShortcut(const QString &id, const QString &shortcut)
{
	instance().deleteShortcutPrivate(id, shortcut);
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

	instance().findShortcut(sequence(instance().currentModifier(), mb));
}

void HotKeyManager::setCurrentModifier(const QString &modifier)
{
	instance().setCurrentModifierPrivate(modifier);
}

QHash<QString, QAction *> HotKeyManager::commands()
{
	return instance().commandsPrivate();
}

QHash<QString, QString> HotKeyManager::shortcuts()
{
	return instance().shortcutsPrivate();
}

void HotKeyManager::registerCommand(const QString &id, QAction *command)
{
	QList<QKeySequence> shortcuts = command->shortcuts();

	foreach (QKeySequence const &shortcut, shortcuts) {
		instance().registerShortcut(id, shortcut.toString());
	}

	mCommands[id] = command;
}

bool HotKeyManager::registerShortcut(QString const &id, QKeySequence const &keyseq)
{
	if (mCommands.contains(id)) {
		QString const shortcut = keyseq.toString();

		if (!hasPrefixOf(shortcut)) {
			addPrefixes(shortcut);
			mShortcuts[shortcut] = id;
			mCommands[id]->setShortcuts(mCommands[id]->shortcuts() << keyseq);
			return true;
		}
	}
	return false;
}

bool HotKeyManager::registerShortcut(QString const &id, QKeySequence const &modifier, MouseButtons mousebutton)
{
	if (mCommands.contains(id)) {
		QString const shortcut = sequence(modifier.toString(), mousebutton);

		if (!mShortcuts.contains(shortcut)) {
			addPrefixes(shortcut);
			mShortcuts[shortcut] = id;
			return true;
		}
	}

	return false;
}

void HotKeyManager::registerShortcut(QString const &id, QString const &shortcut)
{
	if (!hasPrefixOf(shortcut)) {
		addPrefixes(shortcut);
		mShortcuts[shortcut] = id;
	}
}

void HotKeyManager::findShortcut(QString const &shortcut)
{
	if (mShortcuts.contains(shortcut) && mCommands[mShortcuts.value(shortcut)]->parentWidget()->isActiveWindow()) {
		mCommands[mShortcuts.value(shortcut)]->trigger();
	}
}

QString HotKeyManager::sequence(const QString &modifier, MouseButtons mousebutton)
{
	QString seq = modifier;

	switch(mousebutton) {
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

void HotKeyManager::setCurrentModifierPrivate(QString const &modifier)
{
	mCurrentModifer = modifier;
}

QString HotKeyManager::currentModifier()
{
	return mCurrentModifer;
}

void HotKeyManager::resetShortcutsPrivate(QString const &id)
{
	if (mCommands.contains(id)) {
		QList<QString> shortcuts = mShortcuts.keys(id);

		foreach (QString const &shortcut, shortcuts) {
			deletePrefixes(shortcut);
			mShortcuts.remove(shortcut);
		}

		mCommands[id]->setShortcuts(QList<QKeySequence>());
	}
}

void HotKeyManager::resetAllShortcutsPrivate()
{
	QList<QAction *> cmds = mCommands.values();

	foreach (QAction *cmd, cmds) {
		cmd->setShortcuts(QList<QKeySequence>());
	}

	mShortcuts.clear();
	mPrefixes.clear();
}

void HotKeyManager::deleteShortcutPrivate(const QString &id, const QString &shortcut)
{
	mShortcuts.remove(shortcut);
	deletePrefixes(shortcut);
	//if == "" then shortcut with mouse
	if (QKeySequence(shortcut).toString() != "" ) {
		QList<QKeySequence> shortcuts = mCommands[id]->shortcuts();

		shortcuts.removeOne(shortcut);
		mCommands[id]->setShortcuts(shortcuts);
	}
}

void HotKeyManager::deleteCommandPrivate(QString const &id)
{
	if (mCommands.contains(id)) {
		mCommands.remove(id);
	}
}

QHash<QString, QAction *> HotKeyManager::commandsPrivate()
{
	return mCommands;
}

QHash<QString, QString> HotKeyManager::shortcutsPrivate()
{
	return mShortcuts;
}

bool HotKeyManager::hasPrefixOf(QString const &keyseq)
{
	if (!mPrefixes.contains(keyseq)) {
		QStringList seqlist = keyseq.split(", ");
		QString prefix;

		foreach (QString const &seq, seqlist) {
			prefix += seq;
			if (mShortcuts.contains(prefix)) {
				return true;
			}
			prefix += ", ";
		}
		return false;
	}
	return true;
}

void HotKeyManager::addPrefixes(QString const &keyseq)
{
	QStringList seqlist = keyseq.split(", ");
	QString prefix;

	foreach (QString const &seq, seqlist) {
		prefix += seq;
		if (mPrefixes.contains(prefix)) {
			++mPrefixes[prefix];
		} else {
			mPrefixes[prefix] = 1;
		}
		prefix += ", ";
	}
}

void HotKeyManager::deletePrefixes(QString const &keyseq)
{
	QStringList seqlist = keyseq.split(", ");
	QString prefix;

	foreach (QString const &seq, seqlist) {
		prefix += seq;
		--mPrefixes[prefix];
		if (mPrefixes.value(prefix) == 0) {
			mPrefixes.remove(prefix);
		}
		prefix += ", ";
	}
}
