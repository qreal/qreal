#include "hotKeyManager.h"

#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>
#include <QtCore/QEvent>
#include <QtGui/QActionEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QWheelEvent>

using namespace qReal;

HotKeyManager::HotKeyManager()
{
}

HotKeyManager::~HotKeyManager()
{
}

HotKeyManager& HotKeyManager::instance()
{
	static HotKeyManager instance;
	return instance;
}

void HotKeyManager::setCommand(QString const &id, QString const &label, QAction *command)
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

QHash<QString, QAction *> HotKeyManager::commands()
{
	return instance().commandsPrivate();
}

QHash<QString, QString> HotKeyManager::shortcuts()
{
	return instance().shortcutsPrivate();
}

void HotKeyManager::registerCommand(QString const &id, QAction *command)
{
	QList<QKeySequence> const shortcuts = command->shortcuts();

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

void HotKeyManager::resetShortcutsPrivate(QString const &id)
{
	if (mCommands.contains(id)) {
		QStringList const shortcuts = mShortcuts.keys(id);

		foreach (QString const &shortcut, shortcuts) {
			deletePrefixes(shortcut);
			mShortcuts.remove(shortcut);
		}

		mCommands[id]->setShortcuts(QList<QKeySequence>());
	}
}

void HotKeyManager::resetAllShortcutsPrivate()
{
	QList<QAction *> const cmds = mCommands.values();

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

	QList<QKeySequence> shortcuts = mCommands[id]->shortcuts();

	shortcuts.removeOne(shortcut);
	mCommands[id]->setShortcuts(shortcuts);
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
		QStringList const seqlist = keyseq.split(", ");
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
	QStringList const seqlist = keyseq.split(", ");
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
	QStringList const seqlist = keyseq.split(", ");
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
