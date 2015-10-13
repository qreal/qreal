/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

void HotKeyManager::setCommand(const QString &id, const QString &label, QAction *command)
{
	command->setWhatsThis(label);
	instance().registerCommand(id, command);
}

void HotKeyManager::deleteCommand(const QString &id)
{
	resetShortcuts(id);
	instance().deleteCommandPrivate(id);
}

bool HotKeyManager::setShortcut(const QString &id, const QKeySequence &keyseq)
{
	return instance().registerShortcut(id, keyseq);
}

void HotKeyManager::resetShortcuts(const QString &id)
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

void HotKeyManager::registerCommand(const QString &id, QAction *command)
{
	QList<QKeySequence> const shortcuts = command->shortcuts();

	foreach (const QKeySequence &shortcut, shortcuts) {
		instance().registerShortcut(id, shortcut.toString());
	}

	mCommands[id] = command;
}

bool HotKeyManager::registerShortcut(const QString &id, const QKeySequence &keyseq)
{
	if (mCommands.contains(id)) {
		const QString shortcut = keyseq.toString();

		if (!hasPrefixOf(shortcut)) {
			addPrefixes(shortcut);
			mShortcuts[shortcut] = id;
			mCommands[id]->setShortcuts(mCommands[id]->shortcuts() << keyseq);
			return true;
		}
	}
	return false;
}

void HotKeyManager::registerShortcut(const QString &id, const QString &shortcut)
{
	if (!hasPrefixOf(shortcut)) {
		addPrefixes(shortcut);
		mShortcuts[shortcut] = id;
	}
}

void HotKeyManager::findShortcut(const QString &shortcut)
{
	if (mShortcuts.contains(shortcut) && mCommands[mShortcuts.value(shortcut)]->parentWidget()->isActiveWindow()) {
		mCommands[mShortcuts.value(shortcut)]->trigger();
	}
}

void HotKeyManager::resetShortcutsPrivate(const QString &id)
{
	if (mCommands.contains(id)) {
		const QStringList shortcuts = mShortcuts.keys(id);

		foreach (const QString &shortcut, shortcuts) {
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

void HotKeyManager::deleteCommandPrivate(const QString &id)
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

bool HotKeyManager::hasPrefixOf(const QString &keyseq)
{
	if (!mPrefixes.contains(keyseq)) {
		const QStringList seqlist = keyseq.split(", ");
		QString prefix;

		foreach (const QString &seq, seqlist) {
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

void HotKeyManager::addPrefixes(const QString &keyseq)
{
	const QStringList seqlist = keyseq.split(", ");
	QString prefix;

	foreach (const QString &seq, seqlist) {
		prefix += seq;
		if (mPrefixes.contains(prefix)) {
			++mPrefixes[prefix];
		} else {
			mPrefixes[prefix] = 1;
		}
		prefix += ", ";
	}
}

void HotKeyManager::deletePrefixes(const QString &keyseq)
{
	const QStringList seqlist = keyseq.split(", ");
	QString prefix;

	foreach (const QString &seq, seqlist) {
		prefix += seq;
		--mPrefixes[prefix];
		if (mPrefixes.value(prefix) == 0) {
			mPrefixes.remove(prefix);
		}
		prefix += ", ";
	}
}
